"""Point generated Furo edit buttons at the corresponding ArborX source file.

Furo supplies the edit button, but its source URL is derived from Sphinx's
document name. This post-processing step makes the destination repository
explicit and maps the landing page to ``Home.rst``, which contains its prose.
"""

from pathlib import Path
import re
from urllib.parse import quote


SOURCE_DIR = Path(__file__).resolve().parent
BUILD_DIR = SOURCE_DIR.parent / "generated_docs"
REPOSITORY_URL = "https://github.com/kokkos/ArborX"
BRANCH = "docs"
SOURCE_PATH_IN_REPOSITORY = "docs/source"

EDIT_BUTTON = re.compile(
    r'<div class="edit-this-page">.*?</div>', re.DOTALL
)
EDIT_LINK = re.compile(r'href="[^"]*"')
CONTENT_ICON_CONTAINER = '<div class="content-icon-container">'


def edit_url(source_file: Path) -> str:
    """Return the canonical GitHub edit URL for a documentation source."""
    relative_path = source_file.relative_to(SOURCE_DIR).as_posix()
    repository_path = f"{SOURCE_PATH_IN_REPOSITORY}/{relative_path}"
    encoded_path = quote(repository_path, safe="/:@")
    return f"{REPOSITORY_URL}/edit/{quote(BRANCH, safe='')}/{encoded_path}"


def edit_button(url: str) -> str:
    """Build the Furo-compatible edit button used if a page lacks one."""
    return (
        '<div class="edit-this-page">\n'
        f'  <a class="muted-link" href="{url}" rel="edit" '
        'title="Edit this page">\n'
        '    <svg><use href="#svg-pencil"></use></svg>\n'
        '    <span class="visually-hidden">Edit this page</span>\n'
        '  </a>\n'
        '</div>'
    )


def source_for_page(html_file: Path) -> Path | None:
    """Map a generated HTML page to its RST source, if it has one."""
    relative_html_path = html_file.relative_to(BUILD_DIR)
    if relative_html_path == Path("index.html"):
        # index.rst includes Home.rst, where the landing page text lives.
        source_file = SOURCE_DIR / "Home.rst"
    else:
        source_file = SOURCE_DIR / relative_html_path.with_suffix(".rst")
    return source_file if source_file.is_file() else None


def update_edit_button(html_file: Path, url: str) -> None:
    """Update the existing Furo button or add one when the theme omits it."""
    html = html_file.read_text(encoding="utf-8")
    match = EDIT_BUTTON.search(html)
    if match:
        button = match.group(0)
        link_match = EDIT_LINK.search(button)
        if link_match:
            button = EDIT_LINK.sub(f'href="{url}"', button, count=1)
        else:
            button = edit_button(url)
        html = html[: match.start()] + button + html[match.end() :]
    elif CONTENT_ICON_CONTAINER in html:
        html = html.replace(
            CONTENT_ICON_CONTAINER,
            CONTENT_ICON_CONTAINER + "\n" + edit_button(url),
            1,
        )
    else:
        return
    html_file.write_text(html, encoding="utf-8")


def main() -> None:
    if not BUILD_DIR.is_dir():
        raise SystemExit(f"Sphinx output directory does not exist: {BUILD_DIR}")

    updated = 0
    for html_file in sorted(BUILD_DIR.rglob("*.html")):
        source_file = source_for_page(html_file)
        if source_file is None:
            continue
        update_edit_button(html_file, edit_url(source_file))
        updated += 1

    print(f"Updated edit links for {updated} ArborX documentation pages.")


if __name__ == "__main__":
    main()

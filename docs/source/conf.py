project = "ArborX"
copyright = "ArborX contributors"
author = "ArborX contributors"

root_doc = "index"
source_suffix = ".rst"

extensions = [
    "myst_parser",
    "sphinx_copybutton",
    "sphinx_design",
]

cpp_id_attributes = ["KOKKOS_FUNCTION", "KOKKOS_INLINE_FUNCTION"]

exclude_patterns = ["_build", "generated_docs", "_Sidebar.rst", "Home.rst"]

html_theme = "furo"
html_title = "ArborX documentation"
html_logo = "../logos/arborx_logo_v1.0.svg"
html_favicon = "../logos/arborx_logo_v1.0.svg"
html_static_path = ["_static"]
html_css_files = ["arborx.css"]

html_theme_options = {
    "source_repository": "https://github.com/kokkos/ArborX/",
    "source_branch": "master",
    "source_directory": "docs/source/",
}

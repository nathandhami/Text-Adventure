find_package(Sphinx)
find_package(LATEX COMPONENTS PDFLATEX)
find_package(Make)

if (SPHINX_EXECUTABLE)

# Sphinx cache with pickled ReST documents
set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")
 
# HTML output directory
set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")

# LaTeX output directory
set(SPHINX_LATEX_DIR "${CMAKE_CURRENT_BINARY_DIR}/latex")

file(GLOB RST_SOURCES "*.rst")

function(create_docs FORMAT OUTPUTS OUTPUT_DIR)
  add_custom_command(
      OUTPUT ${OUTPUTS}
      COMMAND ${SPHINX_EXECUTABLE}
                -q -b "${FORMAT}"
                -c "${CMAKE_CURRENT_SOURCE_DIR}"
                -d "${SPHINX_CACHE_DIR}"
                   "${CMAKE_CURRENT_SOURCE_DIR}"
                   "${OUTPUT_DIR}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      DEPENDS "conf.py" ${RST_SOURCES}
  )
  add_custom_target("docs_${FORMAT}" ALL
      DEPENDS ${OUTPUTS}
      COMMENT "Building ${FORMAT} documentation with Sphinx"
  )
endfunction(create_docs)


# HTML documentation generation
create_docs("html" "${SPHINX_HTML_DIR}/index.html" "${SPHINX_HTML_DIR}")


# LaTeX and PDF documentation generation
if (PDFLATEX_COMPILER AND MAKE_EXECUTABLE)
  create_docs("latex" "${SPHINX_LATEX_DIR}/pdfsources.tex" "${SPHINX_LATEX_DIR}")

  add_custom_command(
      OUTPUT "${SPHINX_LATEX_DIR}/manual.pdf"
      COMMAND ${MAKE_EXECUTABLE} -C "${SPHINX_LATEX_DIR}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${SPHINX_LATEX_DIR}/manual.pdf" "${SPHINX_HTML_DIR}/"
      DEPENDS "${SPHINX_LATEX_DIR}/pdfsources.tex"
  )
  add_custom_target(docs_pdf ALL
      DEPENDS "${SPHINX_LATEX_DIR}/manual.pdf"
      COMMENT "Building PDF documentation with Sphinx"
  )
endif (PDFLATEX_COMPILER AND MAKE_EXECUTABLE)


endif (SPHINX_EXECUTABLE)


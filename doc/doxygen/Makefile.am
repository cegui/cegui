EXTRA_DIST = BuildDocs.bat \
             arch_overview.dox \
             authors.dox \
             changelog.dox \
             code_standards.dox \
             compiling.dox \
             data_licensing.dox \
             devel.dox \
             downloading.dox \
             doxyfile \
             doxyfile.in \
             futurechanges.dox \
             glew-lic.dox \
             licensing.dox \
             mainpage.dox \
             pcre-lic.dox \
             porting6to7.dox \
             stringencoders-lic.dox \
             tinyxml-lic.dox \
             tolua++README.dox \
			 xml_animation.dox \
             xml_confg.dox \
             xml_font.dox \
             xml_imageset.dox \
             xml_layout.dox \
             xml_scheme.dox \
             falagard/fal_baseclass_ref.dox \
             falagard/fal_element_ref.dox \
             falagard/fal_enum_ref.dox \
             falagard/fal_intro.dox \
             falagard/fal_main.dox \
             falagard/fal_tut1.dox \
             falagard/fal_wr_ref.dox \
             falagard/gnufdl.dox \
             tutorials/ce_guide_1.dox \
             tutorials/ce_guide_2.dox \
             tutorials/ce_guide_3.dox \
             tutorials/ce_guide_4.dox \
             tutorials/rp_setup.dox

html-local:
	@if test "x$(DOXYGEN)" = "x"; then \
		echo "Doxygen was not found by configure."; \
		echo "Install doxygen or explicitly set the DOXYGEN variable to specify the doxygen command location, then try again."; \
		false; \
	fi

	@if test "x$(DOXYGENDOT)" = "x"; then \
		echo "The dot command was not found.  HTML documentation will not include graphs."; \
		echo "Set the DOXYGENDOT variable to explicity specify the dot command location when running configure."; \
	fi

	$(DOXYGEN) doxyfile

install-html-local:
	test -z "$(htmldir)/html" || $(MKDIR_P) "$(DESTDIR)$(htmldir)/html"
	@$(am__vpath_adj_setup) \
	list=`ls -1 html`; for p in $$list; do \
	  d=html/; \
	  $(am__vpath_adj) \
	  echo " $(INSTALL_DATA) '$$d$$p' '$(DESTDIR)$(htmldir)/html/$$f'"; \
	  $(INSTALL_DATA) "$$d$$p" "$(DESTDIR)$(htmldir)/html/$$f"; \
	done

uninstall-local:
	@$(am__vpath_adj_setup) \
	$(am__vpath_adj) \
	echo " rm -rf '$(DESTDIR)$(htmldir)/html'"; \
	rm -rf "$(DESTDIR)$(htmldir)/html";

clean-local:
	@rm -rf html

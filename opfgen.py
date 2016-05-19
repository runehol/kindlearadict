

import os
import os.path
from xml.sax.saxutils import escape, quoteattr


class KindleDictGenerator:
    def __init__(self, title, identifier, creators, in_language, out_language, cover_image, title_page, out_dir, out_filename):
        self.title = title
        self.identifier = identifier
        self.creators = creators
        self.in_language = in_language
        self.out_language = out_language
        self.cover_image = cover_image
        self.title_page = title_page
        self.out_dir = out_dir
        self.out_filename = out_filename

        os.makedirs(out_dir, exist_ok=True)

        self.dict_html_pages = []
        self.dict_html_idx = 0
        self.entries_in_curr_dict_html = 0
        self.max_entries_per_dict_html = 500
        self.curr_dict_f = None

        self.start_dict_html()

        self.index_size = 0
        self.n_orig_entries = 0
        self.n_expanded_entries = 0
        self.n_empty_entries = 0


    def add_dict_entry_internal(self, formatted_head_word, forms, formatted_desc):
        self.n_expanded_entries += 1
        self.index_size += len(forms)

        if self.entries_in_curr_dict_html >= self.max_entries_per_dict_html:
            self.close_dict_html()
            self.start_dict_html()

        self.entries_in_curr_dict_html += 1
                                                    
        self.curr_dict_f.write("""
        <idx:entry scriptable="yes" spell="yes">
          <idx:short>
            <idx:orth value=%s>%s
        """ % (quoteattr(forms[0]), formatted_head_word))

        if len(forms[1:]) > 0:
            self.curr_dict_f.write("""
            <idx:infl>
            """)

            for infl in forms[1:]:
                self.curr_dict_f.write("""<idx:iform value=%s exact="yes"/>\n""" % quoteattr(infl))

            self.curr_dict_f.write("""
            </idx:infl>
            """)

        self.curr_dict_f.write("""
           </idx:orth>
           %s
         </idx:short>
	</idx:entry>
        """ % formatted_desc)


    def add_dict_entry(self, formatted_head_word, forms, formatted_desc):
        if len(forms) == 0:
            self.n_empty_entries += 1
            return
        self.n_orig_entries += 1
        max_n_forms = 250 #yeah, kindlegen has a max length of 255 inflections per entry. we solve that by splitting up the number of entries!
        while len(forms):
            self.add_dict_entry_internal(formatted_head_word, forms[:max_n_forms], formatted_desc)
            forms = forms[max_n_forms:]
        




    def start_dict_html(self):
        dname = "dict%03d" % (self.dict_html_idx)
        self.dict_html_idx += 1
        fname = dname + ".html"
        self.dict_html_pages.append( (dname, fname) )
        self.entries_in_curr_dict_html = 0

        self.curr_dict_f = open(os.path.join(self.out_dir, fname), "w")
        self.curr_dict_f.write("""<html xmlns:math="http://exslt.org/math" xmlns:svg="http://www.w3.org/2000/svg"
	  xmlns:tl="http://www.kreutzfeldt.de/tl"
	  xmlns:saxon="http://saxon.sf.net/" xmlns:xs="http://www.w3.org/2001/XMLSchema"
	  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	  xmlns:cx="http://www.kreutzfeldt.de/mmc/cx"
	  xmlns:dc="http://purl.org/dc/elements/1.1/"
	  xmlns:mbp="http://www.kreutzfeldt.de/mmc/mbp"
	  xmlns:mmc="http://www.kreutzfeldt.de/mmc/mmc"
	  xmlns:idx="http://www.mobipocket.com/idx">
  <head><meta http-equiv="Content-Type" content="text/html; charset=utf-8"></head> <body>
	<mbp:frameset>
""")



    def close_dict_html(self):
        self.curr_dict_f.write("""
	</mbp:frameset>
  </body>
</html>
""")


        self.curr_dict_f.close()
        self.curr_dict_f = None






    def finalize(self):
        self.close_dict_html()
        
        with open(os.path.join(self.out_dir, self.out_filename), "w") as f:
            f.write("""
            <?xml version="1.0?><!DOCTYPE package SYSTEM "oeb1.ent">
            <package uniq-identifier="uid" xmlns:dc="Dublin Core">
""")

            f.write("""
              <metadata>
                <dc-metadata>
                  <dc:Identifier id="uid">%s</dc:Identifier>
                  <dc:Title><h2>%s</h2></dc:Title>
                  <dc:language>%s</dc:language>
            """ % (escape(self.identifier), escape(self.title), escape(self.out_language)))

            for creator in self.creators:
                f.write("""
                  <dc:creator>%s</dc:creator>
            """ % (escape(creator)))
                
            
            f.write("""
                </dc-metadata>
            """)
	
            f.write("""
                <x-metadata>
                  <output encoding="utf-8" flatten-dynamic-dir="yes"/>
                  <DictionaryInLanguage>%s</DictionaryInLanguage>
                  <DictionaryOutLanguage>%s</DictionaryOutLanguage>
                </x-metadata>

                <guide>
                  <reference type="title-page" title="Title" href=%s />
                </guide>
            """ % (escape(self.in_language), escape(self.out_language), quoteattr(self.title_page)))

            f.write("""
              </metadata>
            """)

            f.write("""
              <manifest>
                <item id="aradict-cover" href=%s media-type="image/jpeg" properties="cover-image" />
                <item id="title-page" href=%s media-type="text/x-oeb1-document" />
            """ % (quoteattr(self.cover_image), quoteattr(self.title_page)))
            

            for dictname, dictfile in self.dict_html_pages:
                f.write("""
                  <item id=%s href=%s media-type="text/x-oeb1-document" />
                """ % (quoteattr(dictname), quoteattr(dictfile)))
                

            f.write("""
               </manifest>
            """)

            f.write("""
               <spine toc="My_Table_of_Contents">
                 <!-- the spine defines the linear reading order of the book -->
                 <itemref idref="title-page"/>
""")
            

            for dictname, dictfile in self.dict_html_pages:
                f.write("""
                  <itemref idref=%s/>
                """ % (quoteattr(dictname)))
                

            f.write("""
               </spine>
            """)


            f.write("""
            </package>
            """)

            

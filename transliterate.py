# -*- coding: utf-8 -*-

import re
import itertools

# helper functions for converting from Buckwalter to Unicode and vice versa

buck = "'|>&<}AbptvjHxd*rzs$SDTZEg_fqklmnhwYyFNKaui~o0123456789`{"
unic = "".join(map(chr,
             list(range(0x0621, 0x063b)) + # hamza through ghayn
             list(range(0x0640, 0x0653)) + # taTwiil through sukuun
             list(range(0x0660, 0x066A)) + # numerals
             list(range(0x0670, 0x0672)))) # dagger 'alif, waSla

# more reader-friendly transliteration - ALA/Wehr 
ala  = [# hamza through ghayn
        "ʾ","ʾā","ʾ","ʾ","ʾ","ʾ","ā","b","h","t","ṯ","j","ḥ","ḵ",
        "d","ḏ","r","z","s","š","ṣ","ḍ","ṭ","ẓ","ʿ","ḡ",
        # taTwiil through sukuun
        "","f","q","k","l","m","n","h","w","ā","y",
        "an","un","in","a","u","i","~","",
        # numerals
        "0","1","2","3","4","5","6","7","8","9",
        # dagger 'alif, waSla
        "ā",""]


unic = "".join(map(chr,
             list(range(0x0621, 0x063b)) + # hamza through ghayn
             list(range(0x0640, 0x0653)) + # taTwiil through sukuun
             list(range(0x0660, 0x066A)) + # numerals
             list(range(0x0670, 0x0672)))) # dagger 'alif, waSla

unic_no_vowels = ([chr(c) for c in range(0x0621, 0x063b)]   +  # hamza through ghayn
                  [chr(c) for c in range(0x0640, 0x064b)]   +  # taTwiil through ye
                  [None   for c in range(0x064b, 0x0652)] +  # fathathaan through shadda
                  [None   for c in range(0x0652, 0x0653)]        +  # sukun
                  [chr(c) for c in range(0x0660, 0x066A)]   +  # numerals
                  [chr(c) for c in range(0x0670, 0x0672)])     # dagger 'alif, waSla


optional_vowels = ([(chr(c),) for c in range(0x0621, 0x063b)]   +  # hamza through ghayn
                   [(chr(c),) for c in range(0x0640, 0x064b)]   +  # taTwiil through ye
                   [(chr(c),"") for c in range(0x064b, 0x0652)] +  # fathathaan through shadda
                   [(chr(c),"") for c in range(0x0652, 0x0653)]        +  # sukun
                   [(chr(c),) for c in range(0x0660, 0x066A)]   +  # numerals
                   [(chr(c),) for c in range(0x0670, 0x0672)])     # dagger 'alif, waSla



assert len(buck) == len(unic) == len(ala) == len(unic_no_vowels) == len(optional_vowels)


buck2unic = dict(list(zip([ord(letter) for letter in buck], unic)))
unic2buck = dict(list(zip([ord(letter) for letter in unic], buck)))

buck2optional_vowels = dict(list(zip(buck, optional_vowels)))

unic2novowels = dict(list(zip([ord(letter) for letter in unic], unic_no_vowels)))

def b2u(buckwalter_string):
    string = str(buckwalter_string).translate(buck2unic)
    return string.replace("\u0671", "\u0627") 
    # because alef wasla doesn't show up properly in most fonts

def u2b(unicode_string):
    return unicode_string.translate(unic2buck)

def b2ala_letter(letter):
    try:
        return ala[buck.index(letter)]
    except:
        return letter


def b2u_vowelled_unvowelled_combinations(buckwalter_string):
    combos = [buck2optional_vowels[c] for c in buckwalter_string]
    opt_combos = combos[:1]
    for c in combos[1:]:
        if len(c) == 1 and len(opt_combos[-1]) == 1:
            opt_combos[-1] = (opt_combos[-1][0] + c[0],)
        else:
            opt_combos.append(c)

    return map("".join, itertools.product(*opt_combos))


def unicode_strip_vowels(ustr):
    return ustr.translate(unic2novowels)

def b2ala(buckwalter_string):
    # deal with shadda (doubled letters)
    string = re.sub(r"(.)~", r"\1\1", buckwalter_string)

    string = "".join([b2ala_letter(letter) for letter in string])

    # deal with uw, iy - make them ū and ī
    # might not be 100% correct - should they remain "uw", "iy" in some contexts?
    string = string.replace("uw", "ū")
    string = string.replace("iy", "ī")
    
    # deal with repeated vowels    
    string = string.replace("aan", "an") # nunated alif
    string = string.replace("āa", "ā")
    string = string.replace("ūu", "ū")

    # special case - 3 l's in Allah because there are 2 written l's 
    # and one has a shadda
    string = string.replace("lll", "ll")

    return string

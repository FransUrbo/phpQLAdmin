<?php
// ----------------------------
// pql_lang.inc
// phpQLAdmin Language Application Programming Interface (API)
//
// $Id: pql_lang.inc,v 2.1 2003-06-28 15:11:30 turbo Exp $
//

// ----------------------------
// This file is stolen from the phpRecipeBook project by 
// Todd Rogers <nazgul26@users.sourceforge.net>
// http://sourceforge.net/projects/phprecipebook
//
// Adopted to suit phpQLAdmin by Turbo Fredriksson
// Function and class name(s) have been altered to
// be more in line with the rest of the phpQLAdmin
// package.

define( "UPPER",		1);
define( "LOWER",		2);
define( "UPPERFIRST",	3);
define( "LOWERFIRST",	4);

class pql_lang {
	// =================================================== Private section
	var $_supported = array('en' => 'English');
	var $_language	= NULL;
	
	function _set_array($larr) {
		// set the lang array for translations
		$this->_language =& $larr;
	}

	function _get_supported() {
		return $this->_supported;
	}

	// Get default language from database
	function _get_database_language($ldap) {
		foreach($ldap->ldap_basedn as $dn) {
			$dn   = urldecode($dn);
			
			$sr   = @ldap_read($ldap->ldap_linkid, $dn, "language=*", array("language"));
			if(!$sr) return false;

			$info = ldap_get_entries($ldap->ldap_linkid, $sr)
			  or pql_errormsg($linkid);

			if($info["count"])
			  // We got a hit. Return the very first entry
			  // They _should_ (all) be equal (if they're not, that's a bug).
			  return $info[0]["language"][0];
		}
	}

	// Get default language from browser
	function _get_browser_language() {
        $server_langs = $_SERVER['HTTP_ACCEPT_LANGUAGE'];
        $server_langs = split(';', $server_langs);
        $langs		  = split(',', $server_langs[0]);

        foreach($langs as $key => $value) {
			if(preg_match("/(.*?)-.*/", $value))
			  $langs[$key] = $value;

			foreach($this->_supported as $item => $desc) {
				$value = trim($value);
				if($item == $value)
				  // We support this language, use it
				  return $value;
			}
		}

        return NULL;
	}

	// =================================================== Public section

	// Constructor.
	function pql_lang($ldap) {
		// Get the default language from the database
		$lang = $this->_get_database_language($ldap);
		if(!$lang) {
			// We have no default language in the database -> check the browser.

			// Get the language(s) the browser supports
			$lang = $this->_get_browser_languages();
		}

		if(!$lang)
		  // We have a language (either from the database or the browser)
		  include("include/lang.".$lang.".inc");
		else
		  // No default language. Load the English default
		  include("include/lang.en.inc");

		// Initiate the language array.
		$this->_set_array($language);
	}
	
	// Translate string to the local language [same form as the gettext abbreviation]
	// This is the order of precedence:
	//	* If the key exists in the lang array, return the value of the key
	//	* If no key exists and the base lang is the same as the local lang,
	//	  just return the string
	//	* If this is not the base lang, then return string with a red star
	//	  appended to show that a translation is required.
	function _($str, $case = 0) {
		$x = @$this->_language[$str];
		if($x)
		  // We have a translation for this string -> return it.
		  $str = $x;

		switch($case) {
		  case UPPER:
			$str = strtoupper($str);
			break;

		  case LOWER:
			$str = strtolower($str);
			break;

		  case UPPERFIRST:
			$str = strtoupper($str[0]);
			break;

		  case LOWERFIRST:
			$str = strtolower($str[0]);
			break;
		}

		return $str;
	}
}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>

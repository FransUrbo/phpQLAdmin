<?php
// attribute plugin for spamAssassin
// $Id: attrib.spamassassin.inc,v 2.1 2004-10-13 17:47:29 turbo Exp $

// {{{ attribute_save($request)
function attribute_save($request) {
	global $_pql, $LANG;

	$entry  = $request;
	$attrib = $request["attrib"];

	// ------------------------------------
	// Remove some stuff that shouldn't be in the LDAP object
	unset($entry["rootdn"]); unset($entry["domain"]);
	unset($entry["user"]); unset($entry["PHPSESSID"]);
	unset($entry["commit"]); unset($entry["view"]);
	unset($entry["attrib"]);

	// ------------------------------------
	// Convert the language array, to a flat variable
	if(is_array($request["ok_languages"])) {
		unset($entry["ok_languages"]);

		foreach($request["ok_languages"] as $lang)
		  $entry["ok_languages"] .= $lang . " ";
	}
	
	// ------------------------------------
	// Convert 'on' to '1'.
	if(@$request["rewrite_subject"] == 'on')
	  $entry["rewrite_subject"] = 1;

	// ------------------------------------
	// If we haven't supplied BOTH of the values,
	// we don't use ANY of it.
	if(!$request["spam_level_stars"] or !$entry["spam_level_char"]) {
		unset($entry["spam_level_stars"]);
		unset($entry["spam_level_char"]);
	}

	// ------------------------------------
	// Create a {white,black} listing variable
	if(is_array($request["addresses"])) {
		unset($entry["addresses"]);

		for($i=0; $request["addresses"][$i];) {
			$tag = $request["addresses"][$i];

			$entry[$tag][] = $request["addresses"][$i+1];
			$i = $i+2;
		}
	}

	// ------------------------------------
	// Just incase user didn't press 'Add new value',
	// add the last value that's in the input box.
	unset($entry["key"]); unset($entry["email"]);
	if($request["key"] and $request["email"]) {
		$tag = $request["key"];
		$entry[$tag][] = $request["email"];
	}

	// ------------------------------------
	// Convert this to an array that can be put in a
	// 'spamAssassin' attribute.
	foreach($entry as $key => $value) {
		if(is_array($value)) {
			foreach($value as $list => $val)
			  $new[$attrib][] = $key . " " . $val;
		} else
		  $new[$attrib][] = $key . " " . $value;
	}

	// ------------------------------------
	// Update the user object, replacing the whole
	// attribute set.
	if(pql_replace_attribute($_pql->ldap_linkid, urldecode($request["user"]), '', $new))
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
								   array('what' => $LANG->_('Antispam configuration')));
	else
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $LANG->_('Antispam configuration')))
		. ": " . ldap_error($_pql->ldap_linkid);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>

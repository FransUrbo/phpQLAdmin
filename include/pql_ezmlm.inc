<?php
// ----------------------------
// pql_ezmlm.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_ezmlm.inc,v 1.1 2002-12-20 02:00:49 turbo Exp $
function pql_get_ezmlm_host($linkid) {
    // rearrange the array

    for($i = 0; $linkid->mailing_lists[$i]["host"]; $i++) {
	$host = $linkid->mailing_lists[$i]["host"];
	$name = $linkid->mailing_lists[$i]["local"];

	foreach($linkid->mailing_lists[$i] as $key => $value) {
	    if(is_array($value)) {
		foreach($value as $key => $text) {
		    $hosts[$host][$name]["texts"][$key] = $text;
		}
	    } else {
		$hosts[$host][$name][$key] = $value;
	    }
	}
    }
    
    return $hosts;
}
?>

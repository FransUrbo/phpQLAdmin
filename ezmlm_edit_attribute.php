<?php
// $Id: ezmlm_edit_attribute.php,v 1.1 2002-12-21 12:29:33 turbo Exp $
//
session_start();

require("include/pql.inc");
require("include/pql_ezmlm.inc");

// Ezmlm mailing list manager class(es) and depends
// http://www.phpclasses.org/browse.html/package/177
require("ezmlmmgr/library/forms.php");
require("ezmlmmgr/library/common/tableclass.php");
require("ezmlmmgr/library/links.php");
require("ezmlmmgr/library/ezmlm/editezmlmlistclass.php");

// Initialize
$ezmlm = new edit_ezmlm_list_class();

require("ezmlm-hardcoded.php");
include("header.html");

// Get the list of mailinglists
if(!($ezmlm->load())) {
    $error = $ezmlm->error;
}

// Convert the array we got from ezmlm->load().
if($hosts = pql_get_ezmlm_host($ezmlm)) {
    if(is_array()) {
    } else {
		if($hosts[$domain][$list][$attrib]) {
			echo "<b>MODIFY</b> attribute <u>$attrib</u> on list <u>$list@$domain</u>.<br>Current value: <b>" . $hosts[$domain][$list][$attrib] . "</b><br>";
		} else {
			echo "<b>MODIFY</b> attribute <u>$attrib</u> on list <u>$list@$domain</u>.<br>Current value: <i>not defined</i><br>";
		}
	}
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

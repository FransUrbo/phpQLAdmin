<?php
// $Id: ezmlm_edit_attribute.php,v 1.2 2002-12-22 20:28:00 turbo Exp $
//
session_start();

require("include/pql.inc");
require("include/pql_ezmlm.inc");

// Initialize
$ezmlm = new ezmlm();
require("ezmlm-hardcoded.php");

include("header.html");

// Load list of mailinglists
if($ezmlm->readlists()) {
	if($ezmlm->mailing_lists[$listno][$attrib]) {
		echo "<b>MODIFY</b> attribute <u>$attrib</u> on list <u>".$ezmlm->mailing_lists[$listno]["name"]."@$domain</u>.<br>Current value: <b>" . $ezmlm->mailing_lists[$listno][$attrib] . "</b><br>";
	} else {
		echo "<b>MODIFY</b> attribute <u>$attrib</u> on list <u>".$ezmlm->mailing_lists[$listno]["name"]."@$domain</u>.<br>Current value: <i>not defined</i><br>";
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

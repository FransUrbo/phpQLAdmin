<?php
// $Id: ezmlm_del_attribute.php,v 1.3 2002-12-23 11:46:26 turbo Exp $
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
		$ list = $ezmlm->mailing_lists[$listno]["name"] . "@$domain";

		// TODO
		echo "<b>DELETE</b> attribute <u>$attrib</u> on list <u>$list</u>.<br>Current value: <b>" . $ezmlm->mailing_lists[$listno][$attrib] . "</b><br>";
    } else {
		echo "Can't delete attribute, it's undefined!<br>";
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

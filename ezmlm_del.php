<?php
// $Id: ezmlm_del.php,v 1.5 2002-12-25 01:13:06 turbo Exp $
//
session_start();

require("./include/pql.inc");
require("./include/pql_ezmlm.inc");

// Initialize
$ezmlm = new ezmlm('/usr/bin', 'alias', '/var/lists');

include("./header.html");

// Load list of mailinglists
if($ezmlm->readlists()) {
	if(is_array($ezmlm->mailing_lists[$listno])) {
		$list = $ezmlm->mailing_lists[$listno]["name"] . "@" . $ezmlm->mailing_lists[$listno]["host"];

		// TODO
		echo "<b>DELETE</b> list <u>$list</u>.<br>";
	} echo {
		echo "List does not exists<br>";
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

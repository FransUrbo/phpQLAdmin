<?php
// navigation bar - ezmlm mailinglists manager
// $Id: left-ezmlm.php,v 2.1 2002-12-18 16:40:16 turbo Exp $
//
session_start();

require("pql.inc");
require("pql_control.inc");
require("pql_control_plugins.inc");

// We'll probably be needing to load the 
// Ezmlm mailing list manager class
// http://www.phpclasses.org/browse.html/package/177

require("left-head.html");

$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS, false, 0);
?>
  <!-- EZMLM Mailinglists -->
  <div id="el1Parent" class="parent">
    <a class="item" href="home.php">
      <font color="black" class="heada"><b>Mailinglists</b></font>
    </a>
  </div>

<?php
require("left-trailer.html");
?>

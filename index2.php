<?php
// $Id: index2.php,v 2.39 2004-11-17 07:32:36 turbo Exp $

session_start();
require("./include/pql_config.inc");

if(pql_get_define("PQL_CONF_START_ADVANCED", $_SESSION["USER_DN"])) {
    $_REQUEST["advanced"] = $_SESSION["ADVANCED_MODE"] = 1;
}

$frames = 2; // Default 2 frames - left and main
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// -----------------
// Count how many frames we should open
if(pql_get_define("PQL_CONF_EZMLM_USE")) {
    $frames++;
}

// -----------------
// Should we show the controls frame (ie, is controls configured
// in ANY of the namingContexts)?
if(pql_get_define("PQL_CONF_CONTROL_USE") and $_SESSION["ALLOW_CONTROL_CREATE"] and 
   ($_SESSION["ADVANCED_MODE"] or $_REQUEST["advanced"])) {
    $frames++;
    
    // We need this value for the quota change at least/well...
    // include/attrib.{control.ldapdefaultquota,mailquota}.inc
    $_SESSION["USE_CONTROLS"] = 1;
}

// -----------------
// Calculate left framesizes depending on how many
// frames we're opening. There's a maximum of 5 frames
// (Four on the left + main frame) and a minimum of two
// (one on the left + main frame).
if($frames > 2) {
    $size = 100 / ($frames - 1);
} else {
    $size = 100;
}
$size = sprintf("%d", $size);

// Mozilla have problems with resizing frames.
// By setting a thick border, it's at least
// possible...
if($_SESSION["mozilla"]) {
    $border = 5;
} else {
    $border = 0;
}
?>
<html>
  <head>
    <title><?=pql_get_define("PQL_CONF_WHOAREWE")?></title>
  </head>

  <!-- frames == <?=$frames?> -->

<?php if($_REQUEST["advanced"] and !$_SESSION["SINGLE_USER"]) { // Advance mode - show controls and mailinglist managers ?>
  <frameset cols="260,*" rows="*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 2 -->
    <!-- LEFT frame -->
<?php   if($frames >= 3) { ?>
    <frameset cols="*" rows="<?=$size?>%,*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 3 -->
<?php   } ?>
      <frame src="left.php?advanced=1" name="pqlnav">

<?php   if($frames >= 4) { ?>
      <frameset cols="*" rows="<?=$size?>%,*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 4 -->
<?php   } 

        if($_SESSION["USE_CONTROLS"]) {
?>
      <frame src="left-control.php" name="pqlnavctrl">
<?php   }

        if($frames >= 5) {
?>
      <frameset cols="*" rows="<?=$size?>%,*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 5 -->
<?php   }

       if(pql_get_define("PQL_CONF_EZMLM_USE")) { ?>
      <frame src="left-ezmlm.php"   name="pqlnavezmlm">
<?php   }

        if($frames >= 5) {
?>
      </frameset><!-- $frames >= 5 -->
<?php   }

         if($frames >= 4) {
?>
      </frameset><!-- $frames >= 4 -->
<?php    } 

         if($frames >= 3) {
?>
    </frameset><!-- $frames >= 3 -->
<?php    }  ?>

    <!-- RIGHT frame -->
    <frame src="home.php?advanced=1" name="pqlmain">
  </frameset>
<?php } else {
          // Not running in advanced mode, don't show the
          // controls/mailinglist managers
?>
  <frameset cols="250,*" rows="*" border="0" frameborder="0"> 
    <frame src="left.php?advanced=0" name="pqlnav">
    <frame src="home.php?advanced=0" name="pqlmain">
  </frameset>
<?php } ?>

  <noframes>
    <body bgcolor="#FFFFFF">
    </body>
  </noframes>
</html>

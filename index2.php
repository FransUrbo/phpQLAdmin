<?php
session_start();
require("./include/pql_config.inc");

$frames = 2; // Default 2 frames - left and main
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// -----------------
// Count how many frames we should open
if(pql_get_define("PQL_GLOB_EZMLM_USE")) {
    $frames++;
}

if(pql_get_define("PQL_GLOB_BIND9_USE")) {
    $frames++;
}

// -----------------
// Check if this user is a QmailLDAP/Controls administrator
$controladmins = pql_get_domain_value($_pql, $_pql->ldap_basedn[0], pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR"));
if(is_array($controladmins)) {
    foreach($controladmins as $admin)
      if($admin == $USER_DN)
	$controlsadministrator = 1;
} elseif($controladmins == $USER_DN) {
    $controlsadministrator = 1;
}

// -----------------
// Should we show the controls frame (ie, is controls configured
// in ANY of the namingContexts)?
$counted = 0; // Don't count each of the Control usage more than once
foreach($_pql->ldap_basedn as $dn)  {
    $dn = urldecode($dn);

    $ALLOW_CONTROL_CREATE = 1; // DEBUG

    if(pql_get_define("PQL_GLOB_CONTROL_USE") and $ALLOW_CONTROL_CREATE and $controlsadministrator) {
	$SHOW_FRAME["controls"] = 1;
	if(!$counted) {
	    $frames++;
	    $counted = 1;
	}
    }
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
?>
<html>
  <head>
    <title><?php echo PQL_GLOB_WHOAREWE;?></title>
  </head>

  <!-- frames == <?=$frames?> --!>

<?php if(isset($advanced) and !$SINGLE_USER) { // Advance mode - show controls and mailinglist managers ?>
  <frameset cols="260,*" rows="*" border="1" frameborder="0"><!-- $frames >= 2 --!>
    <!-- LEFT frame --!>
<?php   if($frames >= 3) { ?>
    <frameset cols="*" rows="<?=$size?>%,*" border="1" frameborder="0"><!-- $frames >= 3 --!>
<?php   } ?>
      <frame src="left.php?advanced=1" name="pqlnav">

<?php   if($frames >= 4) { ?>
      <frameset cols="*" rows="<?=$size?>%,*" border="1" frameborder="0"><!-- $frames >= 4 --!>
<?php   } 

        if($SHOW_FRAME["controls"]) {
?>
      <frame src="left-control.php" name="pqlnavctrl">
<?php   }

        if($frames >= 5) {
?>
      <frameset cols="*" rows="<?=$size?>%,*" border="1" frameborder="0"><!-- $frames >= 5 --!>
<?php   }

       if(pql_get_define("PQL_GLOB_EZMLM_USE")) { ?>
      <frame src="left-ezmlm.php"   name="pqlnavezmlm">
<?php   }

        if(pql_get_define("PQL_GLOB_BIND9_USE")) {
?>
      <frame src="left-bind9.php"   name="pqlnavbind9">
<?php   }

        if($frames >= 5) {
?>
      </frameset><!-- $frames >= 5 --!>
<?php   }

         if($frames >= 4) {
?>
      </frameset><!-- $frames >= 4 --!>
<?php    } 

         if($frames >= 3) {
?>
    </frameset><!-- $frames >= 3 --!>
<?php    }  ?>

    <!-- RIGHT frame --!>
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

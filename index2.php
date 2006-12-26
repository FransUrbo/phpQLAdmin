<?php
// $Id: index2.php,v 2.48 2006-12-26 23:08:10 aaron Exp $

require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

if(!@$_SESSION["USER_DN"]) {
  /* The user haven't loged in, force a login */
  pql_header("index.php");
}

if(pql_get_define("PQL_CONF_START_ADVANCED", $_SESSION["USER_DN"])) {
    $_REQUEST["advanced"] = $_SESSION["ADVANCED_MODE"] = 1;
}

$frames = 2; // Default 2 frames - left and main
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// If something changed, make sure we start with these undefined!
$_SESSION["USE_EZMLM"] = 0;

// All these is located in the 'Computers frame'
$_SESSION["USE_CONTROLS"] = 0;
$_SESSION["USE_WEBSRV"] = 0;
$_SESSION["USE_HOSTACL"] = 0;
$_SESSION["USE_SUDO"] = 0;
$_SESSION["USE_AUTOMOUNT"] = 0;
$_SESSION["USE_RADIUS"] = 0;

// -----------------
// Count how many frames we should open
if($_SESSION["ADVANCED_MODE"] or @$_REQUEST["advanced"]) {
  if(pql_get_define("PQL_CONF_EZMLM_USE") and $_SESSION["ALLOW_EZMLM_CREATE"]) {
    $frames++;

    $_SESSION["USE_EZMLM"] = 1;
  }

  // Should we show the controls frame (ie, is controls configured
  // in ANY of the namingContexts)?
  if($_SESSION["ALLOW_CONTROL_CREATE"] or
	 (pql_get_define("PQL_CONF_CONTROL_USE") or
	  pql_get_define("PQL_CONF_WEBSRV_USE") or
	  pql_get_define("PQL_CONF_HOSTACL_USE") or
	  pql_get_define("PQL_CONF_SUDO_USE") or
	  pql_get_define("PQL_CONF_AUTOMOUNT_USE") or
	  pql_get_define("PQL_CONF_RADIUS_USE")))
  {
    $frames++;

	if(pql_get_define("PQL_CONF_CONTROL_USE"))
	  // We need this value for the quota change at least/well...
	  // include/attrib.{control.ldapdefaultquota,mailquota}.inc
	  $_SESSION["USE_CONTROLS"] = 1;

	if(pql_get_define("PQL_CONF_WEBSRV_USE"))
	  $_SESSION["USE_WEBSRV"] = 1;

	if(pql_get_define("PQL_CONF_HOSTACL_USE"))
	  $_SESSION["USE_HOSTACL"] = 1;

	if(pql_get_define("PQL_CONF_SUDO_USE"))
	  $_SESSION["USE_SUDO"] = 1;

	if(pql_get_define("PQL_CONF_AUTOMOUNT_USE"))
	  $_SESSION["USE_AUTOMOUNT"] = 1;

	if(pql_get_define("PQL_CONF_RADIUS_USE"))
	  $_SESSION["USE_RADIUS"] = 1;
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

//echo "Frames: $frames<br>";
//echo "Size: $size<br>";

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

<?php if(@$_REQUEST["advanced"] and !$_SESSION["SINGLE_USER"]) { // Advance mode - show controls and mailinglist managers ?>
  <frameset cols="260,*" rows="*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 2 -->
    <!-- LEFT frame -->
<?php   if($frames >= 3) { ?>
    <frameset cols="*" rows="70%,*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 3 -->
<?php   } ?>
      <frame src="left.php?advanced=1" name="pqlnav">

<?php   if($frames >= 4) { ?>
      <frameset cols="*" rows="50%,*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 4 -->
<?php   } 

        if($_SESSION["USE_CONTROLS"] or $_SESSION["USE_WEBSRV"]    or $_SESSION["USE_HOSTACL"] or
		   $_SESSION["USE_SUDO"]     or $_SESSION["USE_AUTOMOUNT"] or $_SESSION["USE_RADIUS"])
		{
?>
      <frame src="left-control.php" name="pqlnavctrl">
<?php   }

        if($frames >= 5) {
?>
      <frameset cols="*" rows="<?=$size?>%,*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 5 -->
<?php   }

       if($_SESSION["USE_EZMLM"]) { ?>
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
<?php
    $value = pql_get_attribute($_pql->ldap_linkid, $_SESSION["BASE_DN"][0], pql_get_define("PQL_ATTR_START_IN_MY_ACCOUNT"), 0, pql_get_define("PQL_ATTR_START_IN_MY_ACCOUNT").'=*');
    if($value){
?>
   <frame src="user_detail.php?rootdn=<?php
    if(empty($rootdn) and empty($_REQUEST["rootdn"]))
      echo pql_get_rootdn($_SESSION["USER_DN"], 'index2.php');
        $_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);
        ?>&domain=<?php echo 'place domain here'; ?>&user=<?php echo $_SESSION["USER_DN"]; ?>" name="pqlmain">

<?php
    }else{ ?>
       <frame src="home.php?advanced=1" name="pqlmain">
<?php
    } ?>
  </frameset>
<?php } else {
          // Not running in advanced mode, don't show the
          // controls/mailinglist managers
?>
  <frameset cols="250,*" rows="*" border="0" frameborder="0"> 
    <frame src="left.php?advanced=0" name="pqlnav">
<?php
    $value = pql_get_attribute($_pql->ldap_linkid, $_SESSION["BASE_DN"][0], pql_get_define("PQL_ATTR_START_IN_MY_ACCOUNT"), 0, pql_get_define("PQL_ATTR_START_IN_MY_ACCOUNT").'=*');
    if($value){
?>
   <frame src="user_detail.php?rootdn=<?php
    if(empty($rootdn) and empty($_REQUEST["rootdn"]))
      echo pql_get_rootdn($_SESSION["USER_DN"], 'index2.php');
        $_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);
        ?>&domain=<?php echo 'place domain here'; ?>&user=<?php echo $_SESSION["USER_DN"]; ?>" name="pqlmain">
<?php
    }else{ ?>
       <frame src="home.php?advanced=0" name="pqlmain">
<?php
    } ?>

  </frameset>
<?php } ?>

  <noframes>
    <body bgcolor="#FFFFFF">
    </body>
  </noframes>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

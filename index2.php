<?php
// $Id: index2.php,v 2.53 2007-08-16 07:43:13 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

if(!@$_SESSION["USER_DN"]) {
  /* The user haven't loged in, force a login */
  pql_header("index.php", 1);
}

if(pql_get_define("PQL_CONF_START_ADVANCED", $_SESSION["USER_DN"]) and
   ($_SESSION["ADVANCED_MODE"] != 1))
{
  // If we choose to DISABLE advanced mode, this shouldn't be set!
  $_REQUEST["advanced"] = $_SESSION["ADVANCED_MODE"] = 1;
} else {
  $_REQUEST["advanced"] = $_SESSION["ADVANCED_MODE"] = 0;
}

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// If something changed, make sure we start with these undefined!
$_SESSION["USE_USERS"] = 0;
$_SESSION["USE_EZMLM"] = 0;

// All these is located in the 'Computers frame'
$_SESSION["USE_CONTROLS"] = 0;
$_SESSION["USE_WEBSRV"] = 0;
$_SESSION["USE_HOSTACL"] = 0;
$_SESSION["USE_SUDO"] = 0;
$_SESSION["USE_AUTOMOUNT"] = 0;
$_SESSION["USE_RADIUS"] = 0;
// }}}

// {{{ Count how many frames we should open
$frames = 2; // Default 2 frames - left-base and main
foreach($_pql->ldap_basedn as $dn)  {
  // Should users be shown in ANY of the base DN's? Oonly count it once!
  if(pql_get_define("PQL_CONF_SHOW_USERS", $dn) and !$_SESSION["USE_USERS"]) {
	$frames++;
	$_SESSION["USE_USERS"] = 1;
  }
}

if($_SESSION["ADVANCED_MODE"] or @$_REQUEST["advanced"]) {
  $adv_uri = "?advanced=1";
  $size = "22"; // Size of the base frame

  // Should we show the computers frame?
  if($_SESSION["ALLOW_CONTROL_CREATE"] and
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

  if(pql_get_define("PQL_CONF_EZMLM_USE") and $_SESSION["ALLOW_EZMLM_CREATE"]) {
    $frames++;
    $_SESSION["USE_EZMLM"] = 1;
  }
} else {
  $size = "19"; // Size of the base frame
}
//echo "Frames: $frames<br>";
// }}}

// {{{ Mozilla have problems with resizing frames.
// By setting a thick border, it's at least
// possible...
if($_SESSION["mozilla"]) {
    $border = 2;
} else {
    $border = 0;
}
// }}}

$startinmyaccount  = $_pql->get_attribute($_SESSION["USER_DN"], pql_get_define("PQL_ATTR_START_IN_MY_ACCOUNT"),
										  0, pql_get_define("PQL_ATTR_START_IN_MY_ACCOUNT").'=*');
if($startinmyaccount) {
  $_REQUEST["advanced"] = $_SESSION["ADVANCED_MODE"] = 1;
  $adv_uri = "?advanced=1";
}
?>
<html>
  <head>
    <title><?php echo pql_get_define("PQL_CONF_WHOAREWE")?></title>
  </head>

  <!-- frames == <?php echo $frames?> -->

  <frameset cols="260,*" rows="*" border="<?php echo $border?>" frameborder="<?=$border?>"><!-- $frames >= 2 -->
    <!-- LEFT frame -->
<?php if($frames >= 3) { ?>
    <frameset cols="*" rows="<?php echo $size?>%,*" border="<?=$border?>" frameborder="<?=$border?>"><!-- $frames >= 3 -->
<?php } ?>
      <frame src="left-base.php<?php echo $adv_uri?>" name="pqlbase">
<?php if($frames >= 4) { ?>
      <frameset cols="*" rows="70%,*" border="<?php echo $border?>" frameborder="<?=$border?>"><!-- $frames >= 4 -->
<?php } ?>
<?php if($_SESSION["USE_USERS"]) { ?>
        <frame src="left.php<?php echo $adv_uri?>" name="pqlnav">
<?php } ?>

<?php if(@$_REQUEST["advanced"] and !$_SESSION["SINGLE_USER"]) {
		// {{{ Advance mode - show controls and mailinglist managers
		if($frames >= 5) {
?>
        <frameset cols="*" rows="50%,*" border="<?php echo $border?>" frameborder="<?=$border?>"><!-- $frames >= 5 -->
<?php	} 

		if($_SESSION["USE_CONTROLS"] or $_SESSION["USE_WEBSRV"]    or $_SESSION["USE_HOSTACL"] or
		   $_SESSION["USE_SUDO"]     or $_SESSION["USE_AUTOMOUNT"] or $_SESSION["USE_RADIUS"])
		{
?>
        <frame src="left-control.php" name="pqlnavctrl">
<?php	}

		if($frames >= 6) {
?>
        <frameset cols="*" rows="*" border="<?php echo $border?>" frameborder="<?=$border?>"><!-- $frames >= 6 -->
<?php	}

		if($_SESSION["USE_EZMLM"]) { ?>
        <frame src="left-ezmlm.php" name="pqlnavezmlm">
<?php	}

		if($frames >= 6) {
?>
        </frameset><!-- $frames >= 6 -->
<?php	}

		if($frames >= 5) {
?>
        </frameset><!-- $frames >= 5 -->
<?php	} 
// }}}
	  }

	  if($frames >= 4) {
?>
      </frameset><!-- $frames >= 4 -->
<?php }

	  if($frames >= 3) { ?>
    </frameset><!-- $frames >= 3 -->
<?php } ?>

    <!-- RIGHT frame -->
<?php
	  // {{{ Right frame
      if($startinmyaccount) {
?>
    <frame src="user_detail.php?rootdn=<?php
    if(empty($rootdn) and empty($_REQUEST["rootdn"]))
      echo pql_get_rootdn($_SESSION["USER_DN"], 'index2.php');
    $_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);
?>&domain=<?php echo 'place domain here'; ?>&user=<?php echo $_SESSION["USER_DN"]; ?>" name="pqlmain">
<?php } else { ?>
    <frame src="home.php<?php echo $adv_uri?>" name="pqlmain">
<?php }
// }}}
?>
  </frameset>

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

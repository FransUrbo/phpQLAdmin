<?php
session_start();
require("./include/pql_config.inc");
global $config;

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
foreach($_pql->ldap_basedn as $dn)  {
    // Check to see if we have write access for the EZMLM/Controls
    // information in ANY of the domain (one is enough to show the
    // frame!)

    if($config["PQL_CONF_EZMLM_USE"][$dn] == 'true')
      $SHOW_FRAME["ezmlm"] = 1;

    if($config["PQL_CONF_CONTROL_USE"][$dn] and $ALLOW_CONTROL_CREATE[$dn] == 'true')
      $SHOW_FRAME["controls"] = 1;
}
?>
<html>
  <head>
    <title><?php echo PQL_CONF_WHOAREWE;?></title>
  </head>

<?php
if(isset($advanced)) {
    // Advance mode - show controls and mailinglist managers
?>
  <frameset cols="250,*" rows="*" border="1" frameborder="0">
<?php
	if((!$SINGLE_USER and $SHOW_FRAME["ezmlm"]) or $SHOW_FRAME["controls"]) {
?>
    <frameset cols="*" rows="60%,*" border="1" frameborder="0">
<?php
	}
?>
      <frame src="left.php?advanced=1" name="pqlnav">
<?php
	if((!$SINGLE_USER and $SHOW_FRAME["ezmlm"]) and $SHOW_FRAME["controls"]) {
?>
      <frameset cols="*" rows="50%,*" border="1" frameborder="0">
<?php
	}

	if($SHOW_FRAME["controls"]) {
?>
        <frame src="left-control.php" name="pqlnavctrl">
<?php
	}

	if(!$SINGLE_USER and $SHOW_FRAME["ezmlm"]) {
?>
        <frame src="left-ezmlm.php" name="pqlnavezmlm">
<?php
	}
	if(!$SINGLE_USER and $SHOW_FRAME["ezmlm"] and $SHOW_FRAME["controls"]) {
?>
      </frameset>
<?php
	}

	if($SHOW_FRAME["controls"]) {
?>
      <frame src="left-control.php" name="pqlnavctrl">
<?php
	}
	if((!$SINGLE_USER and $SHOW_FRAME["ezmlm"]) or $SHOW_FRAME["controls"]) {
?>
    </frameset>
<?php
	}
?>
    <frame src="home.php?advanced=1" name="pqlmain">
  </frameset>
<?php
} else {
    // Not running in advanced mode, don't show the
    // controls/mailinglist managers
?>
  <frameset cols="250,*" rows="*" border="0" frameborder="0"> 
    <frame src="left.php?advanced=0" name="pqlnav">
    <frame src="home.php?advanced=0" name="pqlmain">
  </frameset>
<?php
}
?>

  <noframes>
    <body bgcolor="#FFFFFF">
    </body>
  </noframes>
</html>

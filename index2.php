<?php
session_start();
require("./include/pql_config.inc");
?>
<html>
  <head>
    <title><?php echo PQL_WHOAREWE;?></title>
  </head>

<?php
if(isset($advanced)) {
    // Advance mode - show controls and mailinglist managers
?>
  <frameset cols="250,*" rows="*" border="1" frameborder="0">
<?php
	if(defined("PQL_LDAP_EZMLM_USE") or defined("PQL_LDAP_CONTROL_USE")) {
?>
    <frameset cols="*" rows="60%,*" border="1" frameborder="0">
<?php
	}
?>
      <frame src="left.php?advanced=1" name="pqlnav">
<?php
	if(defined("PQL_LDAP_EZMLM_USE") and defined("PQL_LDAP_CONTROL_USE")) {
?>
      <frameset cols="*" rows="50%,*" border="1" frameborder="0">
<?php
	}

	if(defined("PQL_LDAP_CONTROL_USE")) {
?>
        <frame src="left-control.php" name="pqlnavctrl">
<?php
	}

	if(defined("PQL_LDAP_EZMLM_USE")) {
?>
        <frame src="left-ezmlm.php" name="pqlnavezmlm">
<?php
	}
	if(defined("PQL_LDAP_EZMLM_USE") and defined("PQL_LDAP_CONTROL_USE")) {
?>
      </frameset>
<?php
	}

	if(defined("PQL_LDAP_CONTROL_USE")) {
?>
      <frame src="left-control.php" name="pqlnavctrl">
<?php
	}
	if(defined("PQL_LDAP_EZMLM_USE") or defined("PQL_LDAP_CONTROL_USE")) {
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

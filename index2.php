<?php
session_start();
require("./include/config.inc");
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
    <frameset cols="*" rows="60%,*" border="1" frameborder="0">
      <frame src="left.php?advanced=1" name="pqlnav">
<?php
	if(PQL_LDAP_EZMLM_USE) {
?>
      <frameset cols="*" rows="50%,*" border="1" frameborder="0">
        <frame src="left-control.php" name="pqlnavctrl">
        <frame src="left-ezmlm.php" name="pqlnavezmlm">
      </frameset>
<?php
	} else {
?>
      <frame src="left-control.php" name="pqlnavctrl">
<?php
	}
?>
    </frameset>
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

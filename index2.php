<?php
session_start();
require("config.inc");
?>
<html>
  <head>
    <title><?php echo PQL_WHOAREWE;?></title>
  </head>

  <frameset cols="250,*" rows="*" border="0" frameborder="0"> 
    <frame src="left.php" name="pqlnav">
    <frame src="home.php" name="pqlmain">
  </frameset>

  <noframes>
    <body bgcolor="#FFFFFF">
    </body>
  </noframes>
</html>

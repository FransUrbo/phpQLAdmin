<?php
// $Id: ezmlm.php,v 1.8.2.1 2006-03-14 14:46:30 turbo Exp $
//
require($_SESSION["path"]."/libs/pql_config.inc");
include($_SESSION["path"]."/header.html");
?>  
  <span class="title1">Ezmlm Mailinglist Control</span>

  <br><br>
Here you can manage your ezmlm mailinglists. Mailing lists have to be located under each domain's mail directory (baseMailDir in the domain object) and is organized per domain name in the list frame.<br>

  </body>
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

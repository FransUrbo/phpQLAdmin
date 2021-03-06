<?php
// Delete a user template object
// $Id: config_template_del.php,v 2.11 2007-03-14 12:10:50 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
include($_SESSION["path"]."/header.html");
// }}}
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove template %template%'),
							array("template" => pql_maybe_decode($_REQUEST["rootdn"])))?></span>
  <br><br>
<?php
if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
  // {{{ We're said yes to delete OR we don't care!
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
    die("Should have deleted '<b>".$_REQUEST["rootdn"]."</b>'<br>");
  } else {
    if(!$_pql->delete($_REQUEST["rootdn"])) {
      pql_format_error(1);
      die();
    }

    pql_header("config_detail.php?view=template", 1);
  }
// }}}
} else {
  // {{{ Verify deletion...
?>
  <br><br>
  <img src="images/info.png" width="16" height="16" border="0">
  <?php echo $LANG->_('Are you really sure'); ?>
  <form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="GET">
    <input type="hidden" name="rootdn" value="<?php echo urlencode($_REQUEST["rootdn"])?>">
	
    <input type="submit" name="ok" value="<?php echo $LANG->_('Yes')?>">
    <input type="button" name="back" value="<?php echo $LANG->_('No')?>" onClick="history.back();">
  </form>
<?php
// }}}
}

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

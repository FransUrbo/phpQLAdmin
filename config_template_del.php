<?php
// Delete a user template object
// $Id: config_template_del.php,v 2.1 2005-02-24 17:04:00 turbo Exp $
//
// {{{ Setup session etc
session_start();
require("./include/pql_config.inc");
include($_SESSION["path"]."/header.html");
// }}}
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove template %template%'),
							array("template" => pql_maybe_decode($_REQUEST["rootdn"])))?></span>
<?php
if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
  // We're said yes to delete OR we don't care!
  if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
    die("Should have deleted '<b>".$_REQUEST["rootdn"]."</b>'<br>");
  } else {
    if(!ldap_delete($_pql->ldap_linkid, $_REQUEST["rootdn"])) {
      pql_format_error(1);
      die();
    }

    header("Location: ".$_SESSION["URI"]."config_detail.php?view=template");
  }
} else {
?>
  <br><br>
  <img src="images/info.png" width="16" height="16" border="0">
  <?=$LANG->_('Are you really sure'); ?>
  <form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="GET">
    <input type="hidden" name="rootdn" value="<?=urlencode($_REQUEST["rootdn"])?>">
	
    <input type="submit" name="ok" value="<?=$LANG->_('Yes')?>">
    <input type="button" name="back" value="<?=$LANG->_('No')?>" onClick="history.back();">
  </form>
<?php }
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

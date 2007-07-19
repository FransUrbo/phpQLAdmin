<?php
// Delete a user template object
// $Id: dhcp3_del.php,v 2.1 2007-07-19 10:27:57 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
include($_SESSION["path"]."/header.html");
// }}}

if(@$_REQUEST["subnet"]) {
  // Called from tables/host_details-dhcp3_subnet.inc
  $mask = $_pql->get_attribute($_REQUEST["subnet"], pql_get_define("PQL_ATTR_DHCP3_NETMASK"));

  $tmp = split(',', $_REQUEST["subnet"]);
  $tmp = split('=', $tmp[0]);

  $subnet = $tmp[1].'/'.$mask;

  $what = $LANG->_('DHCP subnetwork').' '.pql_maybe_decode($subnet);

  $DEL_DN = $_REQUEST["subnet"];
} elseif(@$_REQUEST["subhost"]) {
  // Called from tables/host_details-dhcp3_subnet.inc
  $tmp = split(',', $_REQUEST["subhost"]);
  $tmp = split('=', $tmp[0]);

  $subhost = $tmp[1];
  $what = $LANG->_('DHCP subhost').' '.pql_maybe_decode($subhost);

  $DEL_DN = $_REQUEST["subhost"];
} else {
  die(pql_complete_constant($LANG->_('Unknown type in file %file%'),
							array('file' => __FILE__)));
}
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove %what%'),
														array('what' => $what))?></span>
  <br><br>
<?php
if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $DEL_DN)) {
  // {{{ We're said yes to delete OR we don't care!
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
    die("Should have deleted '<b>".$DEL_DN."</b>'<br>");
  } else {
    if(!$_pql->delete($DEL_DN)) {
      pql_format_error(1);
      die();
    }

	if(@$_REQUEST["subnet"]) {
	  pql_header("host_detail.php?view=dhcp3&host=".$_REQUEST["host"]."&ref=physical", 1);
	} else {
	  pql_header("config_detail.php?view=template", 1);
	}
  }
// }}}
} else {
  // {{{ Verify deletion...
?>
  <br><br>
  <img src="images/info.png" width="16" height="16" border="0">
  <?=$LANG->_('Are you really sure'); ?>
  <form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="GET">
<?php if(@$_REQUEST["subnet"]) { ?>
    <input type="hidden" name="subnet" value="<?=urlencode($_REQUEST["subnet"])?>">
<?php } else { ?>
    <input type="hidden" name="rootdn" value="<?=urlencode($_REQUEST["rootdn"])?>">
<?php } ?>
	
    <input type="submit" name="ok" value="<?=$LANG->_('Yes')?>">
    <input type="button" name="back" value="<?=$LANG->_('No')?>" onClick="history.back();">
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

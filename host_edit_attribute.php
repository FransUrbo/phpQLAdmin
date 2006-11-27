<?php
// Edit attribute of a physical host
// $Id: host_edit_attribute.php,v 1.1.2.2 2006-11-27 13:00:41 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
include($_SESSION["path"]."/header.html");
// }}}

// {{{ Forward back to host detail page
function attribute_forward($msg) {
	global $url;

	$link = "host_detail.php?host=".urlencode($_REQUEST["host"])."&view=".$_REQUEST["view"]."&msg=$msg";

	if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	  if(file_exists($_SESSION["path"]."/.DEBUG_PROFILING")) {
		$now = pql_format_return_unixtime();
		echo "Now: <b>$now</b><br>";
	  }

	  echo "<p>If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	  die("<b>".$_SESSION["URI"].$link."</b>");
	} else
	  pql_header($link);
}
// }}}

// {{{ Verify all submitted values
if(@$_REQUEST["submit"]) {
  $error = false;
  $error_text = array();

  if(($_REQUEST["attrib"] == 'radiusfilterid') and
	 (!@$_REQUEST['radius_version'] or !@$_REQUEST['radius_policy']))
  {
	$error = true;
	if(!@$_REQUEST['radius_version'])
	  $error_text['radius_version'] = $LANG->_('Missing');
	if(!@$_REQUEST['radius_policy'])
	  $error_text['radius_policy'] = $LANG->_('Missing');
  }  
}

if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "_REQUEST:";
  printr($_REQUEST);
}
// }}}

if(($error == 'true') or !@$_REQUEST["submit"]) {
  // {{{ Show form
  $host_reference = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));

  $attrib = $_REQUEST["attrib"];
  if(@$_REQUEST[$attrib] and ($_REQUEST["attrib"] == 'radiusfilterid')) {
	$_REQUEST[$attrib] = preg_replace('/\\\"/', '', $_REQUEST[$attrib]);

	// Extract version
	$tmp = preg_replace('/.*version=/', '', $_REQUEST[$attrib]);
	$_REQUEST[radius_version] = preg_replace('/:.*/', '', $tmp);

	// ... and policy
	$_REQUEST[radius_policy] = preg_replace('/.*policy=/', '', $_REQUEST[$attrib]);
  } else {
	// No previous value, suggest some
	$_REQUEST[radius_version] = '1';
	$_REQUEST[radius_policy]  = 'Enterprise User';
  }
?>
    <span class="title1"><?php echo pql_complete_constant($LANG->_('Change %what% for host %host%'), array('what' => $attrib, 'host' => $host_reference)); ?></span>

    <br><br>

    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left">
<?php if($attrib == 'radiusfilterid') { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('RADIUS Version')?></td>
            <td><input type="text" name="radius_version" value="<?=$_REQUEST[radius_version]?>" size="25"></td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('RADIUS Policy')?></td>
            <td><input type="text" name="radius_policy" value="<?=$_REQUEST[radius_policy]?>" size="25"></td>
          </tr>
<?php } else { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$attrib?></td>
            <td><input type="text" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>" size="50"></td>
          </tr>
<?php } ?>
        </th>
      </table>

      <input type="hidden" name="submit"   value="1">
      <input type="hidden" name="oldvalue" value="<?=$_REQUEST[$attrib]?>">
      <input type="hidden" name="attrib"   value="<?=$attrib?>">
      <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
      <input type="hidden" name="host"     value="<?=$_REQUEST["host"]?>">
      <br>
      <input type="submit" value="<?=$LANG->_('Save')?>">
    </form>
  </body>
</html>
<?php
// }}}
} else {
  // {{{ Modify physical server
  if($_REQUEST["attrib"] == 'radiusfilterid')
	$newvalue = '"Enterasys:version='.$_REQUEST['radius_version'].':policy='.$_REQUEST['radius_policy'].'"';
  else
	die('Unknown attribute');

  if(!@$_REQUEST[oldvalue])
	$_REQUEST[$attrib] = '<i>NULL</i>';

  if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["host"], $_REQUEST["attrib"], 1, $newvalue)) {
	$msg = pql_complete_constant($LANG->_('Successfully changed %what% from %old% to %new%'),
								 array('what' => $_REQUEST["attrib"],
									   'old'  => $_REQUEST["oldvalue"],
									   'new'  => $newvalue));
  } else
	$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								 array('what' => $_REQUEST['attrib'])) . ": " . pql_format_error(0);
	
	attribute_forward($msg);
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

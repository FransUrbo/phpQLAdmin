<?php
// control attribute plugin for
// pbstimeout (max number of bytes in message)
//
// $Id: attrib.control.pbstimeout.inc,v 2.8.2.1 2006-03-23 09:13:15 turbo Exp $

// {{{ pbstimeout_check(type)
function pbstimeout_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ pbstimeout_init(host)
// init values
function pbstimeout_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["pbstimeout"] = pql_get_attribute($_pql_control->ldap_linkid,
												pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												pql_get_define("PQL_ATTR_PBS_TIMEOUT"));

	if(@is_null($_REQUEST["pbstimeout"]))
	  return true;
}
// }}}


// {{{ pbstimeout_print_view(host)
function pbstimeout_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbstimeout_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">pbstimeout (<?=$LANG->_('POP before SMTP server timeout')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Seconds')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["pbstimeout"]) { echo $_REQUEST["pbstimeout"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_PBS_TIMEOUT")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_PBS_TIMEOUT")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbstimeout_print_form(void)
function pbstimeout_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">pbstimeout (<?=$LANG->_('POP before SMTP server timeout')?>)
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Seconds')?></td>
          <td><?php echo pql_format_error_span($error["pbstimeout"]); ?><input type="text" name="pbstimeout" value="<?=$_REQUEST["pbstimeout"]?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ pbstimeout_save(type, host)
function pbstimeout_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_PBS_TIMEOUT"), 1, $_REQUEST["pbstimeout"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbstimeout options')));
		else {
		  pql_format_error();
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbstimeout options')));
		}

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ pbstimeout_help(void)
function pbstimeout_help() {
	global $LANG;
?>
    Timeout in seconds until entries in the cache are invalidated.
    Only used by the pbsdbd server.
<?php
}
// }}}

// {{{ pbstimeout_help_cr(void)
function pbstimeout_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>

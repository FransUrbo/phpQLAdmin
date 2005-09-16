<?php
// control attribute plugin for
// outgoingip (max number of bytes in message)
//
// $Id: attrib.control.outgoingip.inc,v 2.8 2005-09-16 06:08:46 turbo Exp $

// {{{ outgoingip_check(type)
function outgoingip_check($type) {
	global $error, $LANG;

	// check each field
	if(@is_array($_REQUEST["outgoingip"])) {
		foreach($_REQUEST["outgoingip"] as $key => $server) {
			if(empty($server))
			  continue;
			
			// it is an ip address
			if(!preg_match("/([0-9]{1,3}\.){3,3}[0-9]{1,1}/", $server)) {
				$is_error = true;
				$error["outgoingip_$key"] = $LANG->_('Invalid');
			} else {
				// ip format ok, check validity of matched ip address
				$ip = explode(".", $server);
				
				foreach($ip as $ip_key => $ip_part) {
					switch((int)$ip_key) {
					  case 0:
					  case 3:
						if(($ip_part <= 0) or ($ip_part > 254)) {
							$is_error = true;
							$error["outgoingip_$key"] = $LANG->_('Invalid');
						}
						break;
						
					  default:
						if(($ip_part < 0) or ($ip_part > 255)) {
							$is_error = true;
							$error["outgoingip_$key"] = $LANG->_('Invalid');
						}
					} // end switch
				} // end foreach
			} // end if !preg_match...
		} // end foreach
	} // end if is_array...

	if($is_error)
	  return false;
	
	return true;
}
// }}}

// {{{ outgoingip_init(host)
// init values
function outgoingip_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["outgoingip"] = pql_get_attribute($_pql_control->ldap_linkid,
												pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												pql_get_define("PQL_ATTR_OUTGOINGIP"));

	if(@is_null($_REQUEST["outgoingip"]))
	  return true;
}
// }}}


// {{{ outgoingip_print_view(host)
function outgoingip_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	outgoingip_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">outgoingip (<?=$LANG->_('IP that qmail-remote binds to')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title">IP</td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["outgoingip"]) { echo $_REQUEST["outgoingip"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_OUTGOINGIP")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_OUTGOINGIP")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ outgoingip_print_form(void)
function outgoingip_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">outgoingip (<?=$LANG->_('IP that qmail-remote binds to')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('IP Address')?></td>
          <td><?php echo pql_format_error_span($error["outgoingip"]); ?><input type="text" name="outgoingip" value="<?=$_REQUEST["outgoingip"]?>"></td>
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

// {{{ outgoingip_save(type, host)
function outgoingip_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_OUTGOINGIP"), 1, $_REQUEST["outgoingip"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('outgoingip options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('outgoingip options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ outgoingip_help(void)
function outgoingip_help() {
	global $LANG;
?>
    IP address the qmail-remote server should bind to.
<?php
}
// }}}

// {{{ outgoingip_help_cr(void)
function outgoingip_help_cr() {
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

<?php
// control attribute plugin for
// pbsip (max number of bytes in message)
//
// $Id: attrib.control.pbsip.inc,v 2.5 2004-10-19 10:40:41 turbo Exp $

// {{{ pbsip_check(type)
function pbsip_check($type) {
	global $error, $LANG;

	// check each field
	if(is_array($_REQUEST["pbsip"])) {
		foreach($_REQUEST["pbsip"] as $key => $server) {
			if($server == "")
			  continue;
			
			// it is an ip address
			if(!preg_match("/([0-9]{1,3}\.){3,3}[0-9]{1,1}/", $server)) {
				$is_error = true;
				$error["pbsip_$key"] = $LANG->_('Invalid');
			} else {
				// ip format ok, check validity of matched ip address
				$ip = explode(".", $server);
				
				foreach($ip as $ip_key => $ip_part) {
					switch((int)$ip_key) {
					  case 0:
					  case 3:
						if($ip_part <= 0 or $ip_part > 254) {
							$is_error = true;
							$error["pbsip_$key"] = $LANG->_('Invalid');
						}
						break;
						
					  default:
						if($ip_part < 0 or $ip_part > 255) {
							$is_error = true;
							$error["pbsip_$key"] = $LANG->_('Invalid');
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

// {{{ pbsip_init(host)
// init values
function pbsip_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["pbsip"] = pql_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
										   pql_get_define("PQL_ATTR_PBS_IP"));

	if(is_null($_REQUEST["pbsip"]))
	  return true;
}
// }}}


// {{{ pbsip_print_view(host)
function pbsip_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbsip_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">pbsip (<?=$LANG->_('POP before SMTP server IP')?>)
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">IP</td>
<?php
	if(!$_REQUEST["pbsip"]) {
?>
        <td><i><?=$LANG->_('Not set')?></i></td>
<?php
	} else {
?>
        <td><?=$_REQUEST["pbsip"]?></td>
<?php
	} // end if is_array
?>
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=pbsip"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbsip_print_form(void)
function pbsip_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">pbsip (<?=$LANG->_('POP before SMTP server IP')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('IP Address')?></td>
          <td><?php echo pql_format_error_span($error["pbsip_0"]); ?><input type="text" name="pbsip" value="<?=$_REQUEST["pbsip"]?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ pbsip_save(type, host)
function pbsip_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_PBS_IP"), '', $_REQUEST["pbsip"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('POP before SMTP server IP address')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('POP before SMTP server IP address')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ pbsip_help(void)
function pbsip_help() {
	global $LANG;
?>
    IP address the POP before SMTP server should bind to.
<?php
}
// }}}

// {{{ pbsip_help_cr(void)
function pbsip_help_cr() {
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

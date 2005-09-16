<?php
// control attribute plugin for
// pbsservers (max number of bytes in message)
//
// $Id: attrib.control.pbsservers.inc,v 2.9 2005-09-16 06:08:49 turbo Exp $

// {{{ pbsservers_check(type)
function pbsservers_check($type) {
	global $error, $LANG;

	// check each field
	if(@is_array($_REQUEST["pbsservers"])) {
		foreach($_REQUEST["pbsservers"] as $key => $server) {
			if(empty($server))
			  continue;
			
			// it is an ip address
			if(!preg_match("/([0-9]{1,3}\.){3,3}[0-9]{1,1}/", $server)) {
				$is_error = true;
				$error["pbsservers_$key"] = $LANG->_('Invalid');
			} else {
				// ip format ok, check validity of matched ip address
				$ip = explode(".", $server);
				
				foreach($ip as $ip_key => $ip_part) {
					switch((int)$ip_key) {
					  case 0:
					  case 3:
						if(($ip_part <= 0) or ($ip_part > 254)) {
							$is_error = true;
							$error["pbsservers_$key"] = $LANG->_('Invalid');
						}
						break;
						
					  default:
						if(($ip_part < 0) or ($ip_part > 255)) {
							$is_error = true;
							$error["pbsservers_$key"] = $LANG->_('Invalid');
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

// {{{ pbsservers_init(host)
// init values
function pbsservers_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["pbsservers"] = pql_get_attribute($_pql_control->ldap_linkid,
												pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												pql_get_define("PQL_ATTR_PBS_SERVERS"));
	
	if(@is_null($_REQUEST["pbsservers"]))
	  return true;
	elseif(!@is_array($_REQUEST["pbsservers"]))
	  $_REQUEST["pbsservers"] = array($_REQUEST["pbsservers"]);
}
// }}}


// {{{ pbsservers_print_view(host)
function pbsservers_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbsservers_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">pbsservers
      <tr class="title">
        <td class="title" colspan="3"><?=$LANG->_('POP before SMTP servers')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_PBS_SERVERS");

	$tbl_txt = array('tit' => $LANG->_('IP'), 'add' => $LANG->_('Add server'), 'del' => $LANG->_('Remove server'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single($_pql_control->ldap_linkid, $tbl_txt, $tbl_url, $_REQUEST["pbsservers"],
							pql_get_define("PQL_ATTR_PBS_SERVERS"), $class);
?>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbsservers_print_form(void)
function pbsservers_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["pbsservers"]); $i++) { ?>
    <input type="hidden" name="pbsservers[<?=$i?>]" value="<?=$_REQUEST["pbsservers"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">pbsservers (<?=$LANG->_('POP before SMTP servers')?>)</th>
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title"><?=$LANG->_('IP Address')?></td>
          <td class="<?=$class?>">
            <?php echo pql_format_error_span($error["pbsservers"][$i]); ?>
            <input type="text" name="pbsservers[<?=$i?>]" size="40">
          </td>
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

// {{{ pbsservers_save(type, host)
function pbsservers_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_PBS_SERVERS"), '', $_REQUEST["pbsservers"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbsservers options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbsservers options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  case "del":
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_PBS_SERVERS"), $_REQUEST["pbsservers"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('PBS server options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('PBS server options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ pbsservers_help(void)
function pbsservers_help() {
	global $LANG;
?>
    A list of IP addresses of running pbsdbd servers.
    This file is only used by the clients pbsadd and pbscheck.
<?php
}
// }}}

// {{{ pbsservers_help_cr(void)
function pbsservers_help_cr() {
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

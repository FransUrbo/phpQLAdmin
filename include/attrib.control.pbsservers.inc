<?php
// control attribute plugin for
// pbsservers (max number of bytes in message)
//
// $Id: attrib.control.pbsservers.inc,v 2.4 2004-10-18 13:39:32 turbo Exp $

// {{{ pbsservers_check(type)
function pbsservers_check($type) {
	global $error, $LANG;

	// check each field
	if(is_array($_REQUEST["pbsservers"])) {
		foreach($_REQUEST["pbsservers"] as $key => $server) {
			if($server == "")
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
						if($ip_part <= 0 or $ip_part > 254) {
							$is_error = true;
							$error["pbsservers_$key"] = $LANG->_('Invalid');
						}
						break;
						
					  default:
						if($ip_part < 0 or $ip_part > 255) {
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
	
	if(is_null($_REQUEST["pbsservers"]))
	  return true;
}
// }}}


// {{{ pbsservers_print_view(host)
function pbsservers_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbsservers_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">pbsservers (<?=$LANG->_('POP before SMTP servers')?>)</th>
      <tr class="title">
        <td>IP</td>
      </tr>
<?php
	if(!is_array($_REQUEST["pbsservers"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["pbsservers"] as $server) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=pql_maybe_idna_decode($server)?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=pbsservers"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Add server')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbsservers_print_form(void)
function pbsservers_print_form() {
	global $error, $LANG;

	if(($_REQUEST["pbscount"] < count($_REQUEST["pbsservers"]) - 1) and count($_REQUEST["pbsservers"]) != 0)
	  $_REQUEST["pbscount"] = count($_REQUEST["pbsservers"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">pbsservers (<?=$LANG->_('POP before SMTP servers')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('IP Address')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["pbsservers_0"]); ?><input type="text" name="pbsservers[0]" value="<?=$_REQUEST["pbsservers"][0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $_REQUEST["pbscount"]; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["pbsservers_$i"]); ?><input type="text" name="pbsservers[<?=$i?>]" value="<?=$_REQUEST["pbsservers"][$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&pbscount=<?=($_REQUEST["pbscount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete a PBS server, remove the value and save')?>.</td>
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

// {{{ pbsservers_save(type, host)
function pbsservers_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_PBS_SERVERS"),
										 $_REQUEST["pbsservers"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbsservers options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbsservers options')))
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

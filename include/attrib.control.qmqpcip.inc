<?php
// control attribute plugin for
// qmqpcip (max number of bytes in message)
//
// $Id: attrib.control.qmqpcip.inc,v 2.11 2007-02-15 12:07:58 turbo Exp $

// {{{ qmqpcip_check(type)
function qmqpcip_check($type) {
	global $error, $LANG;

	// check each field
	if(@is_array($_REQUEST["qmqpcip"])) {
		foreach($_REQUEST["qmqpcip"] as $key => $server) {
			if(empty($server))
			  continue;
			
			// it is an ip address
			if(!preg_match("/([0-9]{1,3}\.){3,3}[0-9]{1,1}/", $server)) {
				$is_error = true;
				$error["qmqpcip_$key"] = $LANG->_('Invalid');
			} else {
				// ip format ok, check validity of matched ip address
				$ip = explode(".", $server);
				
				foreach($ip as $ip_key => $ip_part) {
					switch((int)$ip_key) {
					  case 0:
					  case 3:
						if(($ip_part <= 0) or ($ip_part > 254)) {
							$is_error = true;
							$error["qmqpcip_$key"] = $LANG->_('Invalid');
						}
						break;
						
					  default:
						if(($ip_part < 0) or ($ip_part > 255)) {
							$is_error = true;
							$error["qmqpcip_$key"] = $LANG->_('Invalid');
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

// {{{ qmqpcip_init(host)
// init values
function qmqpcip_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["qmqpcip"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_ATTR_QMQPCIP"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["qmqpcip"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_QMQPCIP"));

	if(!@$_REQUEST["qmqpcip"])
	  return true;
}
// }}}


// {{{ qmqpcip_print_view(host)
function qmqpcip_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	qmqpcip_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
	 <th colspan="3" align="left">qmqpcip (<?=$LANG->_('QMQP Client IP')?>)
<?php $class = pql_format_table(0); ?>
      <tr>
        <td class="title">IP</td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["qmqpcip"]) { echo $_REQUEST["qmqpcip"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_QMQPCIP")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_QMQPCIP")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ qmqpcip_print_form(void)
function qmqpcip_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">qmqpcip (<?=$LANG->_('QMQP Client IP')?>)</th>
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title"><?=$LANG->_('IP Address')?></td>
          <td class="<?=$class?>">
            <?php echo pql_format_error_span($error["qmqpcip"]); ?>
            <input type="text" name="qmqpcip" value="<?=$_REQUEST["qmqpcip"]?>" size="40">
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

// {{{ qmqpcip_save(type, host)
function qmqpcip_save($type, $host)  {
	global $_pql_control, $error, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_QMQPCIP"), '', $_REQUEST["qmqpcip"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_QMQPCIP"), 1, $_REQUEST["qmqpcip"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('qmqpcip options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('qmqpcip options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ qmqpcip_help(void)
function qmqpcip_help() {
	global $LANG;
?>
	IP addresses of QMQP servers, one address per line.
	qmail-qmqpc will try each address in turn until it
	establishes a QMQP connection or runs out of addresses.
<?php
}
// }}}

// {{{ qmqpcip_help_cr(void)
function qmqpcip_help_cr() {
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

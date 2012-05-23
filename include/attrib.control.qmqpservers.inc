<?php
// control attribute plugin for
// qmqpservers (max number of bytes in message)
//
// $Id: attrib.control.qmqpservers.inc,v 2.26 2007-02-15 12:08:01 turbo Exp $

// {{{ qmqpservers_check(type)
function qmqpservers_check($type) {
	global $error, $LANG;

	// check each field
	if(@is_array($_REQUEST["qmqpservers"])) {
		foreach($_REQUEST["qmqpservers"] as $key => $server) {
			if(empty($server))
			  continue;
			
			// it is an ip address
			if(!preg_match("/([0-9]{1,3}\.){3,3}[0-9]{1,1}/", $server)) {
				$is_error = true;
				$error["qmqpservers_$key"] = $LANG->_('Invalid');
			} else {
				// ip format ok, check validity of matched ip address
				$ip = explode(".", $server);
				
				foreach($ip as $ip_key => $ip_part) {
					switch((int)$ip_key) {
					  case 0:
					  case 3:
						if(($ip_part <= 0) or ($ip_part > 254)) {
							$is_error = true;
							$error["qmqpservers_$key"] = $LANG->_('Invalid');
						}
						break;
						
					  default:
						if(($ip_part < 0) or ($ip_part > 255)) {
							$is_error = true;
							$error["qmqpservers_$key"] = $LANG->_('Invalid');
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

// {{{ qmqpservers_init(host)
// init values
function qmqpservers_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["qmqpservers"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
												   pql_get_define("PQL_ATTR_QMQPSERVERS"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["qmqpservers"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_QMQPSERVERS"));

	if(!@$_REQUEST["qmqpservers"])
	  return true;
	elseif(!@is_array($_REQUEST["qmqpservers"]))
	  $_REQUEST["qmqpservers"] = array($_REQUEST["qmqpservers"]);
}
// }}}


// {{{ qmqpservers_print_view(host)
function qmqpservers_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	qmqpservers_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">qmqpservers
      <tr>
        <td class="title" colspan="3"><?php echo $LANG->_('QMQP servers')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_QMQPSERVERS");

	$tbl_txt = array('tit' => $LANG->_('Servers'), 'add' => $LANG->_('Add server'), 'del' => $LANG->_('Remove server'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single( $tbl_txt, $tbl_url, $_REQUEST["qmqpservers"],
							pql_get_define("PQL_ATTR_QMQPSERVERS"), $class);
?>
    </th>
  </table>

<?php
}
// }}}

// {{{ qmqpservers_print_form(void)
function qmqpservers_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["qmqpservers"]); $i++) { ?>
    <input type="hidden" name="qmqpservers[<?php echo $i?>]" value="<?=$_REQUEST["qmqpservers"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">qmqpservers (<?php echo $LANG->_('QMQP servers')?>)</th>
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title"><?php echo $LANG->_('IP Address')?></td>
          <td class="<?php echo $class?>">
            <?php echo pql_format_error_span($error["qmqpservers"][$i]); ?>
            <input type="text" name="qmqpservers[<?php echo $i?>]" size="40">
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?php echo $_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ qmqpservers_save(type, host)
function qmqpservers_save($type, $host)  {
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
									 pql_get_define("PQL_ATTR_QMQPSERVERS"), '', $_REQUEST["qmqpservers"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_QMQPSERVERS"), 1, $_REQUEST["qmqpservers"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('qmqpservers options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('qmqpservers options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		if(pql_modify_attribute($host,
								pql_get_define("PQL_ATTR_QMQPSERVERS"), $_REQUEST["qmqpservers"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('QMQP Server options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('QMQP Server options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ qmqpservers_help(void)
function qmqpservers_help() {
	global $LANG;
?>
	IP addresses of QMQP servers, one address per line.
	qmail-qmqpc will try each address in turn until it
	establishes a QMQP connection or runs out of addresses.
<?php
}
// }}}

// {{{ qmqpservers_help_cr(void)
function qmqpservers_help_cr() {
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

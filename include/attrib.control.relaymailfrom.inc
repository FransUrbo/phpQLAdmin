<?php
// control attribute plugin for
// relaymailfrom (allowed sender addresses for relaying)
//
// $Id: attrib.control.relaymailfrom.inc,v 2.26 2007-02-15 12:08:04 turbo Exp $

// {{{ relaymailfrom_check(type)
function relaymailfrom_check($type) {
	global $error, $LANG;
	
	// check validity
	if(@is_array($_REQUEST["relaymailfrom"])) {
		foreach($_REQUEST["relaymailfrom"] as $key => $badmail) {
		 	if(empty($badmail))
			  continue;
			
			if(preg_match("/^@/", $badmail)) {
				// retry with added user
				if(!pql_check_email("user" . $badmail)) {
					$is_error = true;
					$error["relaymailfrom_$key"] = $LANG->_('Invalid');
				}
			} else {
				if(!pql_check_email($badmail)) {
					$is_error = true;
					$error["relaymailfrom_$key"] = $LANG->_('Invalid');
				}
			} // end if preg_match...
		} // end foreach
	} // end if is_array
	
	if($is_error)
	  return false;
	
	return true;
}
// }}}

// {{{ relaymailfrom_init(host)
function relaymailfrom_init($host) {
	// init values
	global $_pql_control;
	
	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["relaymailfrom"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
													 pql_get_define("PQL_ATTR_RELAYMAILFROM"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["relaymailfrom"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_RELAYMAILFROM"));
	
	if(@is_null($_REQUEST["relaymailfrom"]))
	  return true;
	elseif(!@is_array($_REQUEST["relaymailfrom"]))
	  $_REQUEST["relaymailfrom"] = array($_REQUEST["relaymailfrom"]);
}
// }}}


// {{{ relaymailfrom_print_view(host)
function relaymailfrom_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	relaymailfrom_init($host);
	
	if(@is_array($_REQUEST["relaymailfrom"])) {
		foreach($_REQUEST["relaymailfrom"] as $key => $mail) {
			if(preg_match("/^@/", $mail))
			  $_REQUEST["relaymailfrom"][$key] = "all of " . pql_strip_username($mail);
		}
	}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">relaymailfrom (<?php echo $LANG->_('Allowed senders for relaying', 4)?>)</th>
      <tr class="title">
        <td class="title" colspan="3"><?php echo $LANG->_('Email')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_RELAYMAILFROM");

	$tbl_txt = array('tit' => $LANG->_('Email relays'), 'add' => $LANG->_('Add relay'), 'del' => $LANG->_('Remove relay'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single( $tbl_txt, $tbl_url, $_REQUEST["relaymailfrom"],
							pql_get_define("PQL_ATTR_RELAYMAILFROM"), $class);
?>
    </th>
  </table>
<?php
}
// }}}

// {{{ relaymailfrom_print_form(void)
function relaymailfrom_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["relaymailfrom"]); $i++) { ?>
    <input type="hidden" name="relaymailfrom[<?php echo $i?>]" value="<?php echo $_REQUEST["relaymailfrom"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">relaymailfrom (<?php echo $LANG->_('Allowed senders for relaying', 4)?>)</th>
        <tr class="title">
          <td class="title" colspan="3"><?php echo $LANG->_('Address')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td>
            <?php echo pql_format_error_span($error["relaymailfrom"][$i]); ?>
            <input type="text" name="relaymailfrom[<?php echo $i?>]" size="40">
          </td>
        </tr>

        <tr class="subtitle">
          <td>
            <img src="images/info.png" width="16" height="16" border="0">
            Note: this is a really bad idea, don't use this for a public server !!!
            <br>
            To allow relaying for a whole domain simply leave user empty (eg. @domain.com).
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

// {{{ relaymailfrom_save(type, host)
function relaymailfrom_save($type, $host) {
	global $_pql_control, $LANG;
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
									     pql_get_define("PQL_ATTR_RELAYMAILFROM"), '', $_REQUEST["relaymailfrom"]))
			      $success = 0;
				}
			  } else {
				if(pql_modify_attribute($host,
										pql_get_define("PQL_ATTR_RELAYMAILFROM"), '', $_REQUEST["relaymailfrom"]))
				  $success = 1;
			}

			if($success)
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('relaymailfrom options')));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('relaymailfrom options'))) . ": " . pql_format_error(0);

			attribute_forward($msg);
		    break;
			// }}}

		case "del":
			// {{{ delete value
			if(pql_modify_attribute($host,
									pql_get_define("PQL_ATTR_RELAYMAILFROM"), $_REQUEST["relaymailfrom"], ''))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('ldapclusterhosts options')));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('ldapclusterhosts options')))
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


// {{{ relaymailfrom_help(void)
function relaymailfrom_help() {
	global $LANG;
?>
	This file contains envelope sender addresses that are allowed to relay
	through this server. This is a really bad idea as sender addresses are
	very easy to spoof and you are an open relay then. You should use <u>POP
	before SMTP</u> or <u>SMTP Authentication</u> instead.
	<p>
	<b>Example values</b>:<br>
    joe@yourdomain.com<br>
    @otherdomain.com
<br><br>
The first example allows joe@yourdomain.com to relay, the second one allows all addresses ending with @otherdomain.com to relay.
<?php
}
// }}}

// {{{ relaymailfrom_help_cr()
function relaymailfrom_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>

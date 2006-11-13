<?php
// control attribute plugin for
// badrcptto (blocked local addresses)
//
// $Id: attrib.control.badrcptto.inc,v 2.25.4.1 2006-11-13 07:16:04 turbo Exp $

// {{{ badrcptto_check(type)
function badrcptto_check($type) {
	global $error, $LANG;

	// check validity
	if(@is_array($_REQUEST["badrcptto"])) {
		foreach($_REQUEST["badrcptto"] as $key => $badmail) {
		 	if($badmail == "")
			  continue;
			
			if(!pql_check_email($badmail)) {
				$is_error = true;
				$error["badrcptto_$key"] = $LANG->_('Invalid');
			}
		} // end foreach
	} // end if is_array

	if($is_error)
	  return false;
	
	return true;
}
// }}}

// {{{ badrcptto_init(host)
// init values
function badrcptto_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["badrcptto"] = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
												 pql_get_define("PQL_ATTR_BADRCPTTO"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["badrcptto"] = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_BADRCPTTO"));

	if(@is_null($_REQUEST["badrcptto"]))
	  return true;
	if(!@is_array($_REQUEST["badrcptto"]))
	  $_REQUEST["badrcptto"] = array($_REQUEST["badrcptto"]);
}
// }}}


// {{{ badrcptto_print_view(host)
function badrcptto_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	badrcptto_init($host);

	if(@is_array($_REQUEST["badrcptto"])) {
		foreach($_REQUEST["badrcptto"] as $key => $mail) {
			if(preg_match("/^@/", $mail))
			  $_REQUEST["badrcptto"][$key] = "all of " . pql_strip_username($mail);
		}
	}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">badrcptto
      <tr>
        <td class="title" colspan="3"><?=$LANG->_('Rejected local addresses')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_BADRCPTTO");

	$tbl_txt = array('tit' => $LANG->_('Addresses'), 'add' => $LANG->_('Add address'), 'del' => $LANG->_('Remove address'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single($_pql_control->ldap_linkid, $tbl_txt, $tbl_url, $_REQUEST["badrcptto"],
							pql_get_define("PQL_ATTR_BADRCPTTO"), $class);
?>
    </th>
  </table>
<?php
}
// }}}

// {{{ badrcptto_print_form(void)
function badrcptto_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["badrcptto"]); $i++) { ?>
    <input type="hidden" name="badrcptto[<?=$i?>]" value="<?=$_REQUEST["badrcptto"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">badrcptto (<?=$LANG->_('Rejected local addresses')?>)</th>
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title"><?=$LANG->_('Address')?></td>
          <td class="<?=$class?>">
            <?php echo pql_format_error_span($error["badrcptto"][$i]); ?>
            <input type="text" name="badrcptto[<?=$i?>]" size="40">
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

// {{{ badrcptto_save(type, host)
function badrcptto_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');

		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($_pql_control->ldap_linkid, $host,
									 pql_get_define("PQL_ATTR_BADRCPTTO"), '', $_REQUEST["badrcptto"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($_pql_control->ldap_linkid,
								  pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								  pql_get_define("PQL_ATTR_BADRCPTTO"), '', $_REQUEST["badrcptto"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('badrcptto options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('badrcptto options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		// {{{ delete value
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_BADRCPTTO"), $_REQUEST["badrcptto"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('Bad RCPT to\'s options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Bad RCPT to\'s options')))
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


// {{{ badrcptto_help(void)
function badrcptto_help() {
	global $LANG;
?>
Contains a list of local recipient addresses that are rejected. If the sender has RELAYCLIENT="" set this file has no effect.
<?php
}
// }}}

// {{{ badrcptto_help_cr(void)
function badrcptto_help_cr() {
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

<?php
// control attribute plugin for
// smtpclustercookie (max number of bytes in message)
//
// $Id: attrib.control.ldapclustercookie.inc,v 2.5 2004-10-18 13:39:32 turbo Exp $

// {{{ smtpclustercookie_check(type)
function smtpclustercookie_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ smtpclustercookie_init(host)
// init values
function smtpclustercookie_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["smtpclustercookie"] = pql_get_attribute($_pql_control->ldap_linkid,
													   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													   pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"));

	if(is_null($_REQUEST["smtpclustercookie"]))
	  return true;
}
// }}}


// {{{ smtpclustercookie_print_view(host)
function smtpclustercookie_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	smtpclustercookie_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">smtpclustercookie (<?=$LANG->_('Cluster cookie')?>)
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Cookie')?></td>
<?php
	if(!$_REQUEST["smtpclustercookie"]) {
?>
        <td><i><?=$LANG->_('Not set')?></i></td>
<?php
	} else {
?>
        <td><?=$_REQUEST["smtpclustercookie"]?></td>
<?php
	} // end if is_array
?>
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=smtpclustercookie"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ smtpclustercookie_print_form(void)
function smtpclustercookie_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">smtpclustercookie (<?=$LANG->_('Cluster cookie')?>)
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Cookie')?></td>
          <td><?php echo pql_format_error_span($error["smtpclustercookie"]); ?><input type="text" name="smtpclustercookie" value="<?=$_REQUEST["smtpclustercookie"]?>"></td>
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

// {{{ smtpclustercookie_save(type, host)
function smtpclustercookie_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"),
										 $_REQUEST["smtpclustercookie"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('smtpclustercookie options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('smtpclustercookie options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ smtpclustercookie_help(void)
function smtpclustercookie_help() {
	global $LANG;
?>
    This cookie (random string) is/should be the same on all SMTP/MX clusters.
    It is to prevent outgoing mails looping back because of incorrect or faked
    MX records pointing to some external IP address which qmail-remote does not
    recognise as in reality 'itself' (as seen from the cluster perspective).
    The external IP address can be for example a load balancer or a NAT device.<br>
    <b>NOTE</b>: String will be truncated after 32 characters. If qmail-remote
                 sees the same string advertised in qmail-smtpd greeting it will
                 abort the delivery attempt, bounce the message and report a SMTP
                 loop error.
<?php
}
// }}}

// {{{ smtpclustercookie_help_cr(void)
function smtpclustercookie_help_cr() {
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

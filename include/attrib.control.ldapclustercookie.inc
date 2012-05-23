<?php
// control attribute plugin for
// smtpclustercookie (max number of bytes in message)
//
// $Id: attrib.control.ldapclustercookie.inc,v 2.13 2007-02-15 12:07:22 turbo Exp $

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
	if($host == "Global")
	  $_REQUEST["smtpclustercookie"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
														 pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["smtpclustercookie"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"));

	if(@is_null($_REQUEST["smtpclustercookie"]))
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
    <th colspan="3" align="left">smtpclustercookie (<?php echo $LANG->_('Cluster cookie')?>)
      <tr class="<?php pql_format_table(); ?>">
<?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Cookie')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["smtpclustercookie"]) { echo $_REQUEST["smtpclustercookie"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE")?>&type=add"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">smtpclustercookie (<?php echo $LANG->_('Cluster cookie')?>)
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Cookie')?></td>
          <td><?php echo pql_format_error_span($error["smtpclustercookie"]); ?><input type="text" name="smtpclustercookie" value="<?php echo $_REQUEST["smtpclustercookie"]?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="<?php echo $_REQUEST["type"]?>">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?php echo $_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ smtpclustercookie_save(type, host)
function smtpclustercookie_save($type, $host)  {
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
									 pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"), '', $_REQUEST["smtpclustercookie"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"), '', $_REQUEST["smtpclustercookie"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('smtpclustercookie options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('smtpclustercookie options')))
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

<?php
// control attribute plugin for
// mailSizeMax (The maximum size that a single message may have)
//
// $Id: attrib.control.mailsizemax.inc,v 2.3 2006-12-16 12:02:14 turbo Exp $
//

// {{{ mailsizemax_check(type)
function mailsizemax_check($type) {
	global $error, $LANG;
	
	if($_REQUEST["mailsizemax"] == "") {
	  // Missing
	  $error["mailsizemax"] = $LANG->_('Missing');
	  return false;
	} elseif(!preg_match("/^[0-9].*[0-9]$/", $_REQUEST['mailsizemax'])) {
	  // Not a number - doesn't start nor end with a number at least
	  $error["mailsizemax"] = $LANG->_('Not a number');
	  return false;
	}
	
	return true;
}
// }}}

// {{{ mailsizemax_init(void)
function mailsizemax_init() {
	global $_pql_control;
	
	// Fetch data from LDAP server
	if($host == "Global")
	  $value = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
								 pql_get_define("PQL_ATTR_MAILSIZEMAX"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $value = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_MAILSIZEMAX"));
	
	if(@is_null($value))
	  return true;
}
// }}}


// {{{ mailsizemax_print_view(host)
function mailsizemax_print_view($host) {
	global $_pql_control, $LANG;

	// Init data
	mailsizemax_init();
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">mailsizemax (<?=$LANG->_('The maximum size (in bytes)\nthat a single message may have. Everything\nover this size gets bounced.')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Bytes')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["mailsizemax"]) { echo $_REQUEST["mailsizemax"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=urlencode($host)?>&attrib=<?=pql_get_define("PQL_ATTR_MAILSIZEMAX")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ mailsizemax_print_form(void)
function mailsizemax_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">mailsizemax (<?=$LANG->_('The maximum size (in bytes)\nthat a single message may have. Everything\nover this size gets bounced.')?>)
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Bytes')?></td>
          <td><?php echo pql_format_error_span($error["mailsizemax"]); ?><input type="text" name="mailsizemax" value="<?=$_REQUEST["mailsizemax"]?>"></td>
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

// {{{ mailsizemax_save(type, host)
function mailsizemax_save($type, $host) {
	global $_pql_control, $error, $LANG;
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
									 pql_get_define("PQL_ATTR_mailsizemax"), '', $_REQUEST["mailsizemax"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($_pql_control->ldap_linkid, $host,
								  pql_get_define("PQL_ATTR_MAILSIZEMAX"),
								  1, $_REQUEST["mailsizemax"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('mailsizemax options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('mailsizemax options'))) . ": " . pql_format_error(0);
		
		// this will redirect to control_cat.php and display the message $msg
		attribute_forward($msg);
		break;
		// }}}

   	default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ mailsizemax_help(void)
function mailsizemax_help() {
	global $LANG;
?>
  <?=$LANG->_('mailSizeMax has no influence on the datasize control file for the\nsmtp daemon. The mailSizeMax decision is not take until the mail is\ndelivered localy (by qmail-lspawn)')?>
<?php
}
// }}}

// {{{ mailsizemax_help_cr(void)
function mailsizemax_help_cr() {
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

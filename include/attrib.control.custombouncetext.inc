<?php
// control attribute plugin for
// custombouncetext (additional bounce message)
//
// $Id: attrib.control.custombouncetext.inc,v 2.21.4.2 2006-11-20 19:55:00 turbo Exp $

// {{{ custombouncetext_check(type)
function custombouncetext_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["use_custombouncetext"] == 1 and $_REQUEST["custombouncetext"] == "") {
		$error["custombouncetext"] = $LANG->_('Missing');
		return false;
	}

	return true;
}
// }}}

// {{{ custombouncetext_init(host)
// init values
function custombouncetext_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["custombouncetext"] = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
														pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["custombouncetext"] = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"));
	
	if(@is_null($_REQUEST["custombouncetext"]))
	  $_REQUEST["use_custombouncetext"] = 0;
	else
	  $_REQUEST["use_custombouncetext"] = 1;
}
// }}}


// {{{ custombouncetext_print_view(host)
function custombouncetext_print_view($host) {
	global $_pql_control, $LANG;
	
	// init data
	custombouncetext_init($host);
	
	if($_REQUEST["use_custombouncetext"] == "0")
	  $_REQUEST["custombouncetext"] = "disabled";

	$cn = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_CN"));
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">custombouncetext (<?=$LANG->_('Additional bounce text')?>) for host <u><?=$cn?></u></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Message')?></td>
        <td><?php echo nl2br($_REQUEST["custombouncetext"]); ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=custombouncetext"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ custombouncetext_print_form(void)
function custombouncetext_print_form() {
	global $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">custombouncetext (<?=$LANG->_('Additional bounce text')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Use additional message')?></td>
          <td>
            <input type="radio" name="use_custombouncetext" value="0" <?php if($_REQUEST["use_custombouncetext"] == "0"){ echo "checked"; }?>> <?=$LANG->_('No')?>
            <input type="radio" name="use_custombouncetext" value="1" <?php if($_REQUEST["use_custombouncetext"] == "1"){ echo "checked"; }?>> <?=$LANG->_('Yes')?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Message')?></td>
          <td><?php echo pql_format_error_span($error["custombouncetext"]); ?><textarea cols="40" rows="10" name="custombouncetext"><?=$_REQUEST["custombouncetext"]?></textarea></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Additional custom text in bounce messages, eg. for providing contact information')?>.</td>
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

// {{{ custombouncetext_save(type, host)
function custombouncetext_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;
	
	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($_REQUEST["use_custombouncetext"] == "0")
		  $_REQUEST["custombouncetext"] = "";
		
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($_pql_control->ldap_linkid, $host,
									 pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"), '', $_REQUEST["custombouncetext"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($_pql_control->ldap_linkid, $host,
								  pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"), 1, $_REQUEST["custombouncetext"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('custombouncetext message')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('custombouncetext message')))
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

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>

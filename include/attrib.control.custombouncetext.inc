<?php
// control attribute plugin for
// custombouncetext (additional bounce message)
//
// $Id: attrib.control.custombouncetext.inc,v 2.24 2007-02-15 12:07:19 turbo Exp $

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
	  $_REQUEST["custombouncetext"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
														pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["custombouncetext"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"));
	
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

	$cn = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_CN"));
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">custombouncetext (<?php echo $LANG->_('Additional bounce text')?>) for host <u><?php echo $cn?></u></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Message')?></td>
        <td><?php echo nl2br($_REQUEST["custombouncetext"]); ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=custombouncetext"><img src="images/edit.png" width="12" height="12" border="0"><?php echo $LANG->_('Change options')?></a></td>
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">custombouncetext (<?php echo $LANG->_('Additional bounce text')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Use additional message')?></td>
          <td>
            <input type="radio" name="use_custombouncetext" value="0" <?php if($_REQUEST["use_custombouncetext"] == "0"){ echo "checked"; }?>> <?php echo $LANG->_('No')?>
            <input type="radio" name="use_custombouncetext" value="1" <?php if($_REQUEST["use_custombouncetext"] == "1"){ echo "checked"; }?>> <?php echo $LANG->_('Yes')?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Message')?></td>
          <td><?php echo pql_format_error_span($error["custombouncetext"]); ?><textarea cols="40" rows="10" name="custombouncetext"><?php echo $_REQUEST["custombouncetext"]?></textarea></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo $LANG->_('Additional custom text in bounce messages, eg. for providing contact information')?>.</td>
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
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"), '', $_REQUEST["custombouncetext"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
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

<?php
// control attribute plugin for
// quotawarning (additional quota warning message)
//
// $Id: attrib.control.quotawarning.inc,v 2.24 2007-02-15 12:08:04 turbo Exp $

// {{{ quotawarning_check(type)
function quotawarning_check($type) {
	global $error, $LANG;

	// missing
	if(($_REQUEST["use_quotawarning"] == 1) and @empty($_REQUEST["quotawarning"])) {
		$error["quotawarning"] = $LANG->_('Missing');
		return false;
	}

	return true;
}
// }}}

// {{{ quotawarning_init(host)
// init values
function quotawarning_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["quotawarning"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
													pql_get_define("PQL_ATTR_QUOTA_WARNING"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["quotawarning"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_QUOTA_WARNING"));

	// initialize radio button
	if(!@$_REQUEST["quotawarning"])
	  $_REQUEST["use_quotawarning"] = 0;
	else
	  $_REQUEST["use_quotawarning"] = 1;
}
// }}}


// {{{ quotawarning_print_view(host)
function quotawarning_print_view($host) {
	global $_pql_control, $LANG;
	
	// init data
	quotawarning_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">quotawarning (<?=$LANG->_('Additional quota warning message')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Message')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["quotawarning"]) { echo nl2br($_REQUEST["quotawarning"]); } else { echo '<i>'.$LANG->_('disabled').'</i>'; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_QUOTA_WARNING")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_QUOTA_WARNING")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ quotawarning_print_form(void)
function quotawarning_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">quotawarning (<?=$LANG->_('Additional quota warning message')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Send quota warnings')?></td>
          <td>
            <input type="radio" name="use_quotawarning" value="0" <?php if($_REQUEST["use_quotawarning"] == "0"){ echo "checked"; }?>> no
            <input type="radio" name="use_quotawarning" value="1" <?php if($_REQUEST["use_quotawarning"] == "1"){ echo "checked"; }?>> yes
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Message')?></td>
          <td><?php echo pql_format_error_span($error["quotawarning"]); ?><textarea cols="40" rows="10" name="quotawarning"><?=$_REQUEST["quotawarning"]?></textarea></td>
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

// {{{ quotawarning_save(type, host)
function quotawarning_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if(!@$_REQUEST["use_quotawarning"])
		  $_REQUEST["quotawarning"] = "";
		
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_QUOTA_WARNING"), '', $_REQUEST["quotawarning"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_QUOTA_WARNING"), 1, $_REQUEST["quotawarning"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('quotawarning message')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('quotawarning message')))
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


// {{{ quotawarning_help(void)
function quotawarning_help() {
	global $LANG;
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
?>
	Custom text in quota warning message. If you set to no, no quota warning
	messages will be issued
<?php
}
// }}}

// {{{ quotawarning_help_cr(void)
function quotawarning_help_cr() {
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

<?php
// control attribute plugin for
// badrcptto (blocked local addresses)
//
// $Id: attrib.control.badrcptto.inc,v 2.20.4.1 2005-04-15 10:38:05 turbo Exp $

// {{{ badrcptto_check(type)
function badrcptto_check($type) {
	global $error, $LANG;

	// check validity
	if(is_array($_REQUEST["badrcptto"])) {
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
	$_REQUEST["badrcptto"] = pql_get_attribute($_pql_control->ldap_linkid,
											   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_ATTR_BADRCPTTO"));

	if(is_null($_REQUEST["badrcptto"]))
	  return true;
}
// }}}


// {{{ badrcptto_print_view(host)
function badrcptto_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	badrcptto_init($host);

	if(is_array($_REQUEST["badrcptto"])) {
		foreach($_REQUEST["badrcptto"] as $key => $mail) {
			if(preg_match("/^@/", $mail))
			  $_REQUEST["badrcptto"][$key] = "all of " . pql_strip_username($mail);
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">badrcptto (<?=$LANG->_('Rejected local addresses')?>)</th>
      <tr class="title">
        <td><?=$LANG->_('Email')?></td>
      </tr>

<?php
	if(!is_array($_REQUEST["badrcptto"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["badrcptto"] as $mail) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$mail?></td>
      </tr>

<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=badrcptto"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ badrcptto_print_form(void)
function badrcptto_print_form() {
	global $error, $LANG;

	if(($_REQUEST["badcount"] < count($_REQUEST["badrcptto"]) - 1) and count($_REQUEST["badrcptto"]) != 0)
	  $_REQUEST["badcount"] = count($_REQUEST["badrcptto"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">badrcptto (<?=$LANG->_('Rejected local addresses')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Address')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["badrcptto_0"]); ?><input type="text" name="badrcptto[0]" value="<?=$_REQUEST["badrcptto"][0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $_REQUEST["badcount"]; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["badrcptto_$i"]); ?><input type="text" name="badrcptto[<?=$i?>]" value="<?=$_REQUEST["badrcptto"][$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?=$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&badcount=<?=($_REQUEST["badcount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete an address remove the value and save')?>.</td>
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

// {{{ badrcptto_save(type, host)

function badrcptto_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"], "badrcptto", '',
								$_REQUEST["badrcptto"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('badrcptto options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('badrcptto options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

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

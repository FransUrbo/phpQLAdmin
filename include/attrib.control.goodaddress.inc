<?php
// control attribute plugin for
// goodmailaddr (max number of bytes in message)
//
// $Id: attrib.control.goodaddress.inc,v 2.3 2004-10-15 15:14:10 turbo Exp $

// {{{ goodmailaddr_check(type)
function goodmailaddr_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ goodmailaddr_init(host)
// init values
function goodmailaddr_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["goodmailaddr"] = pql_control_get_attribute($_pql_control->ldap_linkid,
														 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
														 pql_get_define("PQL_ATTR_GOODADDRESS"));

	if(is_null($_REQUEST["goodmailaddr"]))
	  return true;
}
// }}}


// {{{ goodmailaddr_print_view(host)
function goodmailaddr_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	goodmailaddr_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">goodmailaddr (<?=$LANG->_('Localy accepted mail addresses')?>)</th>
      <tr class="title">
        <td>Address(es)</td>
      </tr>
<?php
	if(!is_array($_REQUEST["goodmailaddr"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["goodmailaddr"] as $address) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=pql_maybe_idna_decode($address)?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=goodmailaddr"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ goodmailaddr_print_form(void)
function goodmailaddr_print_form() {
	global $error, $LANG;

	if(($_REQUEST["goodmailaddrcount"] < count($_REQUEST["goodmailaddr"]) - 1) and count($_REQUEST["goodmailaddresses"]) != 0)
	  $_REQUEST["goodmailaddrcount"] = count($_REQUEST["goodmailaddr"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">goodmailaddr (<?=$LANG->_('Localy accepted mail addresses')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Mail address(es)')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["goodmailaddr_0"]); ?><input type="text" name="goodmailaddresses[0]" value="<?=$_REQUEST["goodmailaddresses"][0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $_REQUEST["goodmailaddrcount"]; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["goodmailaddr_$i"]); ?><input type="text" name="goodmailaddresses[<?=$i?>]" value="<?=$_REQUEST["goodmailaddresses"][$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&goodmailaddrcount=<?=($_REQUEST["goodmailaddrcount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete an address, remove the value and save')?>.</td>
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

// {{{ goodmailaddr_save(type, host)
function goodmailaddr_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_GOODADDRESS"),
										 $_REQUEST["goodmailaddr"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('goodmailaddr options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('goodmailaddr options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ goodmailaddr_help(void)
function goodmailaddr_help() {
	global $LANG;
?>
    This file contains local recipient addresses that are always accepted in
    conjunction with sender or recipient verify checks.
    <i>Example: user@domain or @domain</i><br>
    <b>NOTE</b>: This is useful for things like <postmaster@domain.com> and other
          addresses you always want to accept, even if they are not in the
          ldap directory or when a lookup temporarly fails. See RCPTCHECK
          and SENDERCHECK.
<?php
}
// }}}

// {{{ goodmailaddr_help_cr(void)
function goodmailaddr_help_cr() {
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

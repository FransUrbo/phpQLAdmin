<?php
// control attribute plugin for
// goodmailaddr (max number of bytes in message)
//
// $Id: attrib.control.goodaddress.inc,v 2.12 2007-02-15 12:07:20 turbo Exp $

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
	if($host == "Global")
	  $_REQUEST["goodmailaddr"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
													pql_get_define("PQL_ATTR_GOODADDRESS"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["goodmailaddr"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_GOODADDRESS"));

	if(!@$_REQUEST["goodmailaddr"])
	  return true;
	elseif(!@is_array($_REQUEST["goodmailaddr"]))
	  $_REQUEST["goodmailaddr"] = array($_REQUEST["goodmailaddr"]);
}
// }}}


// {{{ goodmailaddr_print_view(host)
function goodmailaddr_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	goodmailaddr_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">goodmailaddr
      <tr>
        <td class="title" colspan="3"><?php echo $LANG->_('Localy accepted mail addresses')?></th></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_GOODADDRESS");

	$tbl_txt = array('tit' => $LANG->_('Addresses'), 'add' => $LANG->_('Add address'), 'del' => $LANG->_('Remove address'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single( $tbl_txt, $tbl_url, $_REQUEST["goodmailaddr"],
							pql_get_define("PQL_ATTR_GOODADDRESS"), $class);
?>
    </th>
  </table>

<?php
}
// }}}

// {{{ goodmailaddr_print_form(void)
function goodmailaddr_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["goodmailaddr"]); $i++) { ?>
    <input type="hidden" name="goodmailaddr[<?php echo $i?>]" value="<?php echo $_REQUEST["goodmailaddr"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">goodmailaddr (<?php echo $LANG->_('Localy accepted mail addresses')?>)
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title"><?php echo $LANG->_('Mail address(es)')?></td>
          <td class="<?php echo $class?>">
            <?php echo pql_format_error_span($error["goodmailaddr"][$i]); ?>
            <input type="text" name="goodmailaddr[<?php echo $i?>]" size="40">
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

// {{{ goodmailaddr_save(type, host)
function goodmailaddr_save($type, $host)  {
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
									 pql_get_define("PQL_ATTR_GOODADDRESS"), '', $_REQUEST["goodmailaddr"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_GOODADDRESS"), '', $_REQUEST["goodmailaddr"]))
			$success = 1;
		}
		
		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('goodmailaddr options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('goodmailaddr options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		// {{{ delete value
		if(pql_modify_attribute($host,
								pql_get_define("PQL_ATTR_GOODADDRESS"), $_REQUEST["goodmailaddr"], ''))
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

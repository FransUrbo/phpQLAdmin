<?php
// control attribute plugin for
// Non-Primary MX host values
//
// $Id: attrib.control.nonprimmx.inc,v 2.2 2005-06-22 13:58:33 turbo Exp $

// {{{ nonprimaryrcpthosts_check(type)
function nonprimaryrcpthosts_check($type) {
  global $error, $LANG;
  
  // dummy function
  return true;
}
// }}}

// {{{ nonprimaryrcpthosts_init(host)
function nonprimaryrcpthosts_init($host) {
  global $_pql_control;
  
  // Fetch data from ldap server
  $_REQUEST["nonprimaryrcpthosts"] = pql_get_attribute($_pql_control->ldap_linkid,
													   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													   pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"));
  
  if(!$_REQUEST["nonprimaryrcpthosts"])
	return true;
  elseif(!is_array($_REQUEST["nonprimaryrcpthosts"]))
	$_REQUEST["nonprimaryrcpthosts"] = array($_REQUEST["nonprimaryrcpthosts"]);
}
// }}}


// {{{ nonprimaryrcpthosts_print_view(host)
function nonprimaryrcpthosts_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	nonprimaryrcpthosts_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('non-primary MX host')))?>
      <tr>
        <td class="title" colspan="3"><?=$LANG->_('Domain names')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS");

	$tbl_txt = array('tit' => $LANG->_('Domains'), 'add' => $LANG->_('Add domain'), 'del' => $LANG->_('Remove domain'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single($_pql_control->ldap_linkid, $tbl_txt, $tbl_url, $_REQUEST["nonprimaryrcpthosts"],
							pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"), $class);
?>
      </th>
    </table>

<?php
}
// }}}

// {{{ nonprimaryrcpthosts_print_form(void)
function nonprimaryrcpthosts_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('non-primary MX host')))?>
        <tr class="title">
          <td><?=$LANG->_('Domain name')?></td>
          <td><input type="text" name="nonprimaryrcpthosts" size="40"></td>
        </tr>
        </th>
     </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="rootdn" value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="type"   value="<?=$_REQUEST["type"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ nonprimaryrcpthosts_save(type, host)
function nonprimaryrcpthosts_save($type, $host) {
    global $_pql, $_pql_control, $LANG;
	$success = 0;

    switch($type) {
	  // {{{ Add entry
	  case "add":
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								$_REQUEST["attrib"], '', $_REQUEST["nonprimaryrcpthosts"]))
		  $msg = pql_complete_constant($LANG->_('Successfully added %domain% to %what%'),
									   array('what'   => $_REQUEST["attrib"],
											 'domain' => $_REQUEST["nonprimaryrcpthosts"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to add %domain% to %what%'),
									   array('what'   => $_REQUEST["attrib"],
											 'domain' => $_REQUEST["nonprimaryrcpthosts"])) . ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;
		// }}}

	  // {{{ Delete entry
      case "del":
		// remove specified domain from locals, redirect back to host details
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								$_REQUEST["attrib"], $_REQUEST["nonprimaryrcpthosts"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully removed %domain% from %what%'),
									   array('what'   => $_REQUEST["attrib"],
											 'domain' => $_REQUEST["nonprimaryrcpthosts"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove %domain% from %what%'),
									   array('what'   => $_REQUEST["attrib"],
											 'domain' => $_REQUEST["nonprimaryrcpthosts"])) . ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;
		// }}}

	  // {{{ Default
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
		// }}}
	} 
}
// }}}


// {{{ nonprimaryrcpthosts_help(void)
function nonprimaryrcpthosts_help() {
  global $LANG;

  echo $LANG->_("This view is \uonly\U for phpQLAdmin. It have nothing to do with QmailLDAP/Controls! It's here so that you can add domain names this hosts only \ulistens\U for, but not accepting mails for.<br>If you do not add your domains here, they might be lost from the RCPTHosts when/if you do a manual replication. Please see <a href='http://bugs.bayour.com/query.php?bug=257' target='_top'>bug #257</a> for more information.");
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>

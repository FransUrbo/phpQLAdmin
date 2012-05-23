<?php
// control attribute plugin for
// Non-Primary MX host values
//
// $Id: attrib.control.nonprimmx.inc,v 2.7 2007-02-15 12:07:38 turbo Exp $

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
  if($host == "Global")
	$_REQUEST["nonprimaryrcpthosts"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
														 pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"), 0, pql_get_define("PQL_ATTR_CN")."=*");
  else
	$_REQUEST["nonprimaryrcpthosts"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"));
  
  if(!@empty($_REQUEST["nonprimaryrcpthosts"]))
	return true;
  elseif(!@is_array($_REQUEST["nonprimaryrcpthosts"]))
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
        <td class="title" colspan="3"><?php echo $LANG->_('Domain names')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS");

	$tbl_txt = array('tit' => $LANG->_('Domains'), 'add' => $LANG->_('Add domain'), 'del' => $LANG->_('Remove domain'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single( $tbl_txt, $tbl_url, $_REQUEST["nonprimaryrcpthosts"],
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('non-primary MX host')))?>
        <tr class="title">
          <td><?php echo $LANG->_('Domain name')?></td>
          <td><input type="text" name="nonprimaryrcpthosts" size="40"></td>
        </tr>
        </th>
     </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?php echo $_REQUEST["mxhost"]?>">
    <input type="hidden" name="rootdn" value="<?php echo $_REQUEST["rootdn"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <input type="hidden" name="type"   value="<?php echo $_REQUEST["type"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ nonprimaryrcpthosts_save(type, host)
function nonprimaryrcpthosts_save($type, $host) {
    global $_pql, $_pql_control, $LANG;
	$success = 0;

    switch($type) {
	  case "add":
		// {{{ Add entry
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 $_REQUEST["attrib"], '', $_REQUEST["nonprimaryrcpthosts"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  $_REQUEST["attrib"], '', $_REQUEST["nonprimaryrcpthosts"]))
			$success = 1;
		}

		if($success)
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

      case "del":
		// {{{ remove specified domain from locals, redirect back to host details
		if(pql_modify_attribute($host,
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

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
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

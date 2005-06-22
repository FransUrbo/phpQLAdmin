<?php
// control attribute plugin for
// ldapclusterhosts (max number of bytes in message)
//
// $Id: attrib.control.ldapclusterhosts.inc,v 2.7 2005-06-22 13:58:33 turbo Exp $

// {{{ ldapclusterhosts_check(type)
function ldapclusterhosts_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ ldapclusterhosts_init(host)
// init values
function ldapclusterhosts_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["ldapclusterhosts"] = pql_get_attribute($_pql_control->ldap_linkid,
													  pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													  pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"));

	if($_REQUEST["ldapclusterhosts"])
	  return true;
	elseif(!is_array($_REQUEST["ldapclusterhosts"]))
	  $_REQUEST["ldapclusterhosts"] = array($_REQUEST["ldapclusterhosts"]);
}
// }}}


// {{{ ldapclusterhosts_print_view(host)
function ldapclusterhosts_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapclusterhosts_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">ldapclusterhosts
      <tr>
        <td class="title" colspan="3"><?=$LANG->_('Hosts in the mail cluster')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS");

	$tbl_txt = array('tit' => $LANG->_('LDAP Cluster hosts'), 'add' => $LANG->_('Add host'), 'del' => $LANG->_('Remove host'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single($_pql_control->ldap_linkid, $tbl_txt, $tbl_url, $_REQUEST["ldapclusterhosts"],
							pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"), $class);
?>

      <tr class="subtitle">
        <td colspan="3"><img src="images/info.png" width="16" height="16" border="0"> Note: Clustering support must be compiled into qmail-ldap</td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ ldapclusterhosts_print_form(void)
function ldapclusterhosts_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
<?php for($i = 0; $_REQUEST["ldapclusterhosts"][$i]; $i++) { ?>
    <input type="hidden" name="ldapclusterhosts[<?=$i?>]" value="<?=$_REQUEST["ldapclusterhosts"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapclusterhosts (<?=$LANG->_('Host to regognize as part of the mail cluster')?>)
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title">FQDN</td>
          <td class="<?=$class?>">
            <?php echo pql_format_error_span($error["ldapclusterhosts"][$i]); ?>
            <input type="text" name="ldapclusterhosts[<?=$i?>]" size="40">
          </td>
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

// {{{ ldapclusterhosts_save(type, host)
function ldapclusterhosts_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"), '', $_REQUEST["ldapclusterhosts"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapclusterhosts options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapclusterhosts options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  case "del":
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"), $_REQUEST["ldapclusterhosts"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapclusterhosts options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapclusterhosts options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ ldapclusterhosts_help(void)
function ldapclusterhosts_help() {
	global $LANG;
?>
    ldapclusterhosts contains a number of hostnames (FQDN - Fully Qualified Domain Name)
    to check together with ~control/me when clustering is on. This file is useful if you
    want to run multiple instances of qmail-ldap on one machine.
<?php
}
// }}}

// {{{ ldapclusterhosts_help_cr(void)
function ldapclusterhosts_help_cr() {
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

<?php
// control attribute plugin for
// ldapclusterhosts (max number of bytes in message)
//
// $Id: attrib.control.ldapclusterhosts.inc,v 2.12 2007-02-15 12:07:23 turbo Exp $

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
	if($host == "Global")
	  $_REQUEST["ldapclusterhosts"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
														pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["ldapclusterhosts"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"));

	if(@$_REQUEST["ldapclusterhosts"])
	  return true;
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
	pql_format_table_single( $tbl_txt, $tbl_url, $_REQUEST["ldapclusterhosts"],
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
<?php for($i=0; $i < count($_REQUEST["ldapclusterhosts"]); $i++) { ?>
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
									 pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"), '', $_REQUEST["ldapclusterhosts"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"), '', $_REQUEST["ldapclusterhosts"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapclusterhosts options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapclusterhosts options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		// {{{ delete value
		if(pql_modify_attribute($host,
								pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"), $_REQUEST["ldapclusterhosts"], ''))
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

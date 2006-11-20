<?php
// control attribute plugin for
// ldapcluster (clustering support)
//
// $Id: attrib.control.ldapcluster.inc,v 2.20.4.3 2006-11-20 19:55:00 turbo Exp $

// {{{ ldapcluster_check(type)
function ldapcluster_check($type) {
	return true;
}
// }}}

// {{{ ldapcluster_init(host)
// init values
function ldapcluster_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["ldapcluster"] = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
												   pql_get_define("PQL_ATTR_LDAPCLUSTER"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["ldapcluster"] = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_LDAPCLUSTER"));

	if(@is_null($_REQUEST["ldapcluster"])) {
		// set to default value
		$_REQUEST["ldapcluster"] = 0;
		return true;
	}
}
// }}}


// {{{ ldapcluster_print_view(host)
function ldapcluster_print_view($host) {
	global $ldapcluster, $view, $LANG;

	// init data
	ldapcluster_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldapcluster (<?=$LANG->_('Clustering support')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Clustering')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["ldapcluster"]) { echo $LANG->_('Yes'); } else { echo $LANG->_('No'); }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_LDAPCLUSTER")?>&<?=pql_get_define("PQL_ATTR_LDAPCLUSTER")?>=<?php if(@$_REQUEST["ldapcluster"]) { echo "0"; } else { echo "1"; }?>&submit=1&type=modify"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_LDAPCLUSTER")?>"></a></td>
      </tr>

      <tr class="subtitle" colspan="3">
        <td colspan="3"><img src="images/info.png" width="16" height="16" border="0"> Note: Clustering support must be compiled into qmail-ldap</td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ ldapcluster_print_form(void)
function ldapcluster_print_form() {
	// don't print form, save directly
	ldapcluster_save("modify", $_REQUEST["mxhost"]);
}
// }}}

// {{{ ldapcluster_save(type, host)
function ldapcluster_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;
	$attrib = $_REQUEST["attrib"];

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($_pql_control->ldap_linkid, $host,
									 pql_get_define("PQL_ATTR_LDAPCLUSTER"), 1, $_REQUEST[$attrib]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($_pql_control->ldap_linkid, $host,
								  pql_get_define("PQL_ATTR_LDAPCLUSTER"), 1, $_REQUEST[$attrib]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapcluster options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapcluster options')))
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

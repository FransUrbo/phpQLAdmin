<?php
// attribute plugin for
// additional domain name
// $Id: attrib.additionaldomainname.inc,v 2.24 2004-05-04 08:16:42 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;

    // is typed in ?
    if(empty($_REQUEST["additionaldomainname"])) {
		$error["additionaldomainname"] = $LANG->_('Missing');
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $_REQUEST["additionaldomainname"])) {
		$error["additionaldomainname"] = $LANG->_('Invalid');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["additionaldomainname"] = @$_REQUEST["oldvalue"];
}
// }}}

// {{{ attribute_print_form(void)

function attribute_print_form() {
    global $error, $LANG;
	global $orgname;			// Set by domain_edit_attributes.php.

?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $LANG->_('additional domain name'), 'domain' => $orgname)); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Additional domain name')?></td>
          <td><?php echo pql_format_error_span($error["additionaldomainname"]); ?><input type="text" name="additionaldomainname" value="<?=$_REQUEST["additionaldomainname"]?>" size="20"></td>
        </tr>
<?php if(!function_exists("pql_control"))
		require("./include/pql_control.inc");

	  // Initiate a connection to the QmailLDAP/Controls DN
	  $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	  $hosts = pql_control_get_hosts($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"]);
	  if(is_array($hosts)) {
?>

        <tr></tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain to '.pql_get_define("PQL_ATTR_LOCALS").' in following objects'))); ?>:</td>
          <td>
<?php		if(!pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"])) {
				$i = 0;

		  		foreach($hosts as $host) {
?>
            <input type="checkbox" name="<?php echo pql_get_define('PQL_ATTR_LOCALS');?>_<?=$i?>" value="<?=$host?>"><?=pql_maybe_idna_decode($host)?><br>
<?php	  			$i++;
		  		}
?>
            <input type="checkbox" name="<?php echo pql_get_define('PQL_ATTR_LOCALS');?>_<?=$i?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('to all hosts'))); ?><br>
            <input type="hidden" name="<?php echo pql_get_define('PQL_ATTR_LOCALS');?>" value="<?=$i?>">
            &nbsp;<?=$LANG->_('If all left unset, no replication will take place')?>
<?php		} else {
				$cnt = count($hosts);
?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
              <input type="hidden" name="<?php echo pql_get_define('PQL_ATTR_LOCALS');?>_<?=$cnt?>" value="on">
              <input type="hidden" name="<?php echo pql_get_define('PQL_ATTR_LOCALS');?>" value="<?=$cnt?>">
<?php		} ?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain to '.pql_get_define('PQL_ATTR_RCPTHOSTS').' in following objects'))); ?>:</td>
          <td>
<?php		if(!pql_get_define("PQL_CONF_CONTROL_AUTOADDRCPTHOSTS", $_REQUEST["rootdn"])) {
		  		foreach($hosts as $host) {
?>
            <input type="checkbox" name="<?php echo pql_get_define('PQL_ATTR_RCPTHOSTS');?>_<?=$i?>" value="<?=$host?>"><?=pql_maybe_idna_decode($host)?><br>
<?php	  			$i++;
		  		}
?>
            <input type="checkbox" name="<?php echo pql_get_define('PQL_ATTR_RCPTHOSTS');?>_<?=$i?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('to all hosts'))); ?><br>
            <input type="hidden" name="<?php echo pql_get_define('PQL_ATTR_RCPTHOSTS');?>" value="<?=$i?>">
            &nbsp;<?=$LANG->_('If all left unset, no replication will take place')?>
<?php 		} else {
				$cnt = count($hosts);
?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
              <input type="hidden" name="<?php echo pql_get_define('PQL_ATTR_RCPTHOSTS');?>_<?=$cnt?>" value="on">
              <input type="hidden" name="<?php echo pql_get_define('PQL_ATTR_RCPTHOSTS');?>" value="<?=$cnt?>">
<?php		} ?>
          </td>
        </tr>
<?php } ?>
<?php if(pql_get_define("PQL_CONF_BIND9_USE")) { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Create DNS template')?></td>
          <td><input type="checkbox" name="template"> Yes</td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["additionaldomainname"]?>">
    <input type="hidden" name="domain"   value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"   value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

// }}}

// {{{ attribute_save(type)

function attribute_save($type) {
    global $_pql, $LANG;

    $attrib = $_REQUEST["attrib"];
	$oldvalue = @$_REQUEST["oldvalue"];
	$additionaldomainname = $_REQUEST["additionaldomainname"];
	$delval = @$_REQUEST["delval"];

	if(($additionaldomainname == $oldvalue) and !$delval) {
		// don't change if new value equal to old one
		$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Additional domain name')));
		attribute_forward($msg);
		return false;
	}

    switch($type) {
      case "add":
      case "modify":
		$delval = $oldvalue;
		// fall through here, so that we ADD this new value

      case "delete":
		// Fetch old values from DB
		$oldvalues = pql_domain_get_value($_pql, $_REQUEST["domain"], $attrib);
		if(is_array($oldvalues)) {
			foreach($oldvalues as $val) {
				if(!@eregi($delval, $val)) {
					// Remove the one we don't want
					$entry[$attrib][] = $val;
				}
			}
		}

		/* DLW: The code that decides whether to delete or add needs to be
		 *      thought through a bit more. */
		if($additionaldomainname)
		  // Add the new value
		  $entry[$attrib][] = pql_maybe_idna_encode($additionaldomainname);
		elseif(!is_array($entry)) 
		  // We're deleting the last entry...
		  $entry[$attrib] = array();

		if(pql_domain_set_value($_pql->ldap_linkid, $_REQUEST["domain"], '', $entry)) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										 array('what' => $LANG->_('Additional domain name')));

			// Create a template DNS zone
			if($_REQUEST["template"] && $additionaldomainname && pql_get_define("PQL_CONF_BIND9_USE")) {
				require("./include/pql_bind9.inc");
				
				if(! pql_bind9_add_zone($_pql->ldap_linkid, $_REQUEST["domain"], $additionaldomainname))
				  $msg = pql_complete_constant($LANG->_("Failed to add domain %domainname%"), array("domainname" => $defaultdomain));
			}

			require("./include/pql_control.inc");
			$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

			// -----------------------------------------
			if(pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"])) {
				// update locals if control patch is enabled and we have decided to do auto replication
				$amount = $_POST[pql_get_define("PQL_ATTR_LOCALS")];
				if($amount) {
					if($_POST[pql_get_define("PQL_ATTR_LOCALS")."_".$amount] != 'on') {
						for($i=0; $i < $amount; $i++)
						  if($_POST[pql_get_define("PQL_ATTR_LOCALS")."_".$i])
							$hosts[] = $_POST[pql_get_define("PQL_ATTR_LOCALS")."_".$i];
						
						if(is_array($hosts)) {
							foreach($hosts as $host)
							  if($type == 'delete')
								pql_control_update_domains($_pql_control, $_REQUEST["rootdn"],
														   pql_get_define("PQL_ATTR_CN").'='.$host.','.$_SESSION["USER_SEARCH_DN_CTR"],
														   '*', array($delval, ''), pql_get_define("PQL_ATTR_LOCALS"));
							  else
								pql_control_update_domains($_pql_control, $_REQUEST["rootdn"],
														   pql_get_define("PQL_ATTR_CN").'='.$host.','.$_SESSION["USER_SEARCH_DN_CTR"],
														   '*', array($oldvalue, $additionaldomainname), pql_get_define("PQL_ATTR_LOCALS"));
						}
					} else {
						// Change all hosts
						if($type == 'delete')
						  pql_control_update_domains($_pql_control, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
													 '*', array($delval, ''),
													 pql_get_define("PQL_ATTR_LOCALS"));
						else
						  pql_control_update_domains($_pql_control, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
													 '*', array($oldvalue, $additionaldomainname),
													 pql_get_define("PQL_ATTR_LOCALS"));
					}
				} else {
					// We have not gone through the form for adding defaultDomain nor additionalDomainName.
					// This _MUST_ be a 'delete {default,additional} domain'...
					//
					// Delete ALL locals/rcpthosts on ALL QmailLDAP/Controls that lists this domain.
					pql_control_update_domains($_pql_control, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
											   '*', array($delval, ''));
				}
			}

			// -----------------------------------------
			if(pql_get_define("PQL_CONF_CONTROL_AUTOADDRCPTHOSTS", $_REQUEST["rootdn"])) {
				// update rcpthosts if control patch is enabled and we have decided to do auto replication
				$amount = $_POST[pql_get_define("PQL_ATTR_RCPTHOSTS")];
				if($amount) {
					if($_POST[pql_get_define("PQL_ATTR_RCPTHOSTS")."_".$amount] != 'on') {
						for($i=0; $i < $amount; $i++)
						  if($_POST[pql_get_define("PQL_ATTR_RCPTHOSTS")."_".$i])
							$hosts[] = $_POST[pql_get_define("PQL_ATTR_RCPTHOSTS")."_".$i];
						
						if(is_array($hosts)) {
							foreach($hosts as $host)
							  if($type == 'delete')
								pql_control_update_domains($_pql_control, $_REQUEST["rootdn"],
														   pql_get_define("PQL_ATTR_CN").'='.$host.','.$_SESSION["USER_SEARCH_DN_CTR"],
														   '*', array($delval, ''),
														   pql_get_define("PQL_ATTR_RCPTHOSTS"));
							  else
								pql_control_update_domains($_pql_control, $_REQUEST["rootdn"],
														   pql_get_define("PQL_ATTR_CN").'='.$host.','.$_SESSION["USER_SEARCH_DN_CTR"],
														   '*', array($oldvalue, $additionaldomainname),
														   pql_get_define("PQL_ATTR_RCPTHOSTS"));
						}
					} else {
						// Change all hosts
						if($type == 'delete')
						  pql_control_update_domains($_pql_control, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
													 '*', array($delval, ''),
													 pql_get_define("PQL_ATTR_RCPTHOSTS"));
						else
						  pql_control_update_domains($_pql_control, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
													 '*', array($oldvalue, $additionaldomainname),
													 pql_get_define("PQL_ATTR_RCPTHOSTS"));
					}
				} else {
					// We have not gone through the form for adding defaultDomain nor additionalDomainName.
					// This _MUST_ be a 'delete {default,additional} domain'...
					//
					// Delete ALL locals/rcpthosts on ALL QmailLDAP/Controls that lists this domain.
					pql_control_update_domains($_pql_control, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
											   '*', array($delval, ''));
				}
			}
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Additional domain name')))
			. ": " . ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;
		
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

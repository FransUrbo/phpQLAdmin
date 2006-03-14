<?php
// attribute plugin for
// additional domain name
// $Id: attrib.additionaldomainname.inc,v 2.33.2.1 2006-03-14 14:46:38 turbo Exp $

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
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $LANG->_('additional domain name'), 'domain' => $_REQUEST["orgname"])); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Additional domain name')?></td>
          <td><?php echo pql_format_error_span($error["additionaldomainname"]); ?><input type="text" name="additionaldomainname" value="<?=$_REQUEST["additionaldomainname"]?>" size="20"></td>
        </tr>
<?php if(!function_exists("pql_control"))
		require($_SESSION["path"]."/libs/pql_control.inc");

	  // Initiate a connection to the QmailLDAP/Controls DN
	  $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	  // Get all QmailLDAP/Control hosts.
      $result = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
						   '(&(cn=*)(objectclass=qmailControl))', 'ONELEVEL');
	  for($i=0; $i < count($result); $i++)
		$hosts[] = pql_get_attribute($_pql_control->ldap_linkid, $result[$i], pql_get_define("PQL_ATTR_CN"));

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
<?php		} else { ?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
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
<?php 		} else { ?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
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
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
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
  
  // {{{ Don't change if new value equal to old one
  if(($additionaldomainname == $oldvalue) and !$delval) {
	$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Additional domain name')));
	attribute_forward($msg);
	return false;
  }
  // }}}
  
  switch($type) {
  case "add":
  case "modify":
	$delval = $oldvalue;
	// fall through here, so that we ADD this new value
	
  case "delete":
	// {{{ Fetch old (attrib) values from DB
	$oldvalues = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], $attrib);
	if($oldvalues) {
	  if(!is_array($oldvalues))
		$oldvalues = array($oldvalues);
	  
	  foreach($oldvalues as $val) {
		if(!@eregi($delval, $val)) {
		  // Remove the one we don't want
		  $entry[$attrib][] = $val;
		}
	  }
	}
	// }}}
	
	if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["domain"], '', '', $entry)) {
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
								   array('what' => $LANG->_('Additional domain name')));
	  
	  // {{{ Create a template DNS zone
	  if($_REQUEST["template"] && $additionaldomainname && pql_get_define("PQL_CONF_BIND9_USE")) {
		require($_SESSION["path"]."/libs/pql_bind9.inc");
		
		if(! pql_bind9_add_zone($_pql->ldap_linkid, $_REQUEST["domain"], $additionaldomainname))
		  $msg = pql_complete_constant($LANG->_("Failed to add domain %domainname%"), array("domainname" => $defaultdomain));
	  }
	  // }}}
	  
	  // {{{ Update locals and/or rcptHosts
	  // .. If control patch is enabled and we have decided to do auto replication
	  require($_SESSION["path"]."/libs/pql_control.inc");
	  $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
	  
	  $attribs = array(pql_get_define("PQL_ATTR_LOCALS")    => pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"]),
					   pql_get_define("PQL_ATTR_RCPTHOSTS") => pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"]));
	  foreach($attribs as $attrib => $autoadd) {
		if($autoadd) {
		  $amount = $_POST[$attrib];
		  if($amount) {
			if($_POST[$attrib."_".$amount] != 'on') {
			  for($i=0; $i < $amount; $i++)
				if($_POST[$attrib."_".$i])
				  $hosts[] = $_POST[$attrib."_".$i];
			  
			  if(is_array($hosts)) {
				foreach($hosts as $host) {
				  $hostdn = pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"];
				  
				  if($type == 'delete')
					pql_modify_attribute($_pql_control->ldap_linkid, $hostdn, $attrib, $delval, '');
				  else
					pql_modify_attribute($_pql_control->ldap_linkid, $hostdn, $attrib, '', $additionaldomainname);
				}
			  }
			} else {
			  // Change all hosts
			  if($type == 'delete')
				pql_modify_attribute($_pql_control->ldap_linkid, $hostdn, $attrib, $delval, '');
			  else
				pql_modify_attribute($_pql_control->ldap_linkid, $hostdn, $attrib, '', $additionaldomainname);
			}
		  } else {
			// We have not gone through the form for adding defaultDomain nor additionalDomainName.
			if(!$delval)
			  // We're ADDING a domain name
			  $entry = array('', $additionaldomainname);
			elseif($additionaldomainname)
			  // We're REPLACING a domain name
			  $entry = array($delval, $additionaldomainname);
			else
			  // We're REMOVING a domain name
			  $entry = array($delval, '');
		  }
		}
	  }
	  
	  pql_control_update_domains($_pql_control, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"], '*', $entry);
	  // }}}
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

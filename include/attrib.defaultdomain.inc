<?php
// attribute plugin for
// default domain name (the default domainname for a domain)
// $Id: attrib.defaultdomain.inc,v 2.40 2007-02-15 12:08:05 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;
    
    // is typed in ?
    if($_REQUEST["defaultdomain"]) {
	  // don't use ".."
	  if(preg_match("/(\.){2,}/", $_REQUEST["defaultdomain"])) {
		$error[pql_get_define("PQL_ATTR_DEFAULTDOMAIN")] = $LANG->_('Invalid');
		return false;
	  }
	}
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["defaultdomain"] = @$_REQUEST["oldvalue"];
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;

	$defaultdomain = $_REQUEST["defaultdomain"];
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('default domain name'), 'domain' => $_REQUEST["orgname"])); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default domain name')?></td>
          <td><?php echo pql_format_error_span($error[pql_get_define("PQL_ATTR_DEFAULTDOMAIN")]); ?><input type="text" name="defaultdomain" value="<?=$defaultdomain?>" size="20"></td>
        </tr>
<?php if(!function_exists("pql_control"))
		require($_SESSION["path"]."/include/pql_control.inc");

	  // Initiate a connection to the QmailLDAP/Controls DN
	  $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
		
      // Get all QmailLDAP/Control hosts.
      $result = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
                           '(&(cn=*)(objectclass=qmailControl))');
      for($i=0; $i < count($result); $i++)
        $hosts[] = $_pql_control->get_attribute($result[$i], pql_get_define("PQL_ATTR_CN"));

	  if(is_array($hosts)) {
?>

        <tr></tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => pql_complete_constant($LANG->_('domain to %what% in following objects'), array('what' => pql_get_define("PQL_ATTR_LOCALS"))))); ?>:</td>
          <td>
<?php		if(!pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"])) {
				$i = 0;

		  		foreach($hosts as $host) {
?>
            <input type="checkbox" name="<?=pql_get_define("PQL_ATTR_LOCALS")?>_<?=$i?>" value="<?=$host?>"><?=$host?><br>
<?php	  			$i++;
		  		}
?>
            <input type="checkbox" name="<?=pql_get_define("PQL_ATTR_LOCALS")?>_<?=$i?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('to all hosts'))); ?><br>
            <input type="hidden" name="<?=pql_get_define("PQL_ATTR_LOCALS")?>" value="<?=$i?>">
            &nbsp;<?=$LANG->_('If all left unset, no replication will take place')?>
<?php		} else { ?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
<?php		} ?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => pql_complete_constant($LANG->_('domain to %what% in following objects'), array('what' => pql_get_define("PQL_ATTR_RCPTHOSTS"))))); ?>:</td>
          <td>
<?php		if(!pql_get_define("PQL_CONF_CONTROL_AUTOADDRCPTHOSTS", $_REQUEST["rootdn"])) {
		  		foreach($hosts as $host) {
?>
            <input type="checkbox" name="<?=pql_get_define("PQL_ATTR_RCPTHOSTS")?>_<?=$i?>" value="<?=$host?>"><?=$host?><br>
<?php	  			$i++;
		  		}
?>
            <input type="checkbox" name="<?=pql_get_define("PQL_ATTR_RCPTHOSTS")?>_<?=$i?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('to all hosts'))); ?><br>
            <input type="hidden" name="<?=pql_get_define("PQL_ATTR_RCPTHOSTS")?>" value="<?=$i?>">
            &nbsp;<?=$LANG->_('If all left unset, no replication will take place')?>
<?php 		} else { ?>
              &nbsp;<?=$LANG->_('Automatic replication configured')?>
<?php 		} ?>
          </td>
        </tr>
<?php	} // if(is_array($hosts)) ?>

        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_("If empty, \bremove\B ('unset') default domain name")?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$defaultdomain?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"     value="<?=@$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
  global $_pql, $LANG;
  
  $oldvalue = @$_REQUEST["oldvalue"];
  $defaultdomain = @$_REQUEST["defaultdomain"];
  
  // {{{ Don't change if new value equal to old one
  if($defaultdomain == $oldvalue) {
	$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Default domainname')));
	attribute_forward($msg);
	break;
  }
  // }}}
  
  switch($type) {
  case "add":
  case "modify":
	if(pql_modify_attribute($_REQUEST["domain"], $_REQUEST["attrib"], 1, $defaultdomain)) {
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
								   array('what' => $LANG->_('Default domainname', 4),
										 'new'  => $defaultdomain));
	  
	  // {{{ Update locals and/or rcptHosts
	  // .. If control patch is enabled and we have decided to do auto replication
	  require($_SESSION["path"]."/include/pql_control.inc");
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

				  pql_modify_attribute($hostdn, $attrib, '', $defaultdomain);
				}
			  }
			} else {
			  // Change all hosts
			  if($type == 'delete')
				pql_modify_attribute($hostdn, $attrib, $oldvalue, '');
			  else
				pql_modify_attribute($hostdn, $attrib, '', $defaultdomain);
			}
		  } else {
			// We have not gone through the form for adding defaultDomain nor additionalDomainName.
			if(!$oldvalue)
			  // We're ADDING a domain name
			  $entry = array('', $defaultdomain);
			elseif($defaultdomain)
			  // We're REPLACING a domain name
			  $entry = array($oldvalue, $defaultdomain);
			else
			  // We're REMOVING a domain name
			  $entry = array($oldvalue, '');
		  }
		}
	  }

	  pql_control_update_domains($_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"], '*', $entry);
	  // }}}
	} else
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $LANG->_('Default domainname'))).": ".ldap_error($_pql->ldap_linkid);
	
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
?>

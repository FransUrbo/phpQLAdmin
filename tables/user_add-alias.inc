<?php /* $Id: user_add-alias.inc,v 1.4 2005-02-24 17:04:02 turbo Exp $ */
      // Retreive domain branches for this domain (for the subbranch value below)
      $branches = pql_unit_get($_pql->ldap_linkid, $_REQUEST["domain"]);
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" accept-charset="UTF-8">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('User data')?></th>
        <!-- source object -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Source DN')?></td>
          <td>
            <?php echo pql_format_error_span($error_text["source"]); ?>
            <?=pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])?>=
            <input type="text" name="source" value="<?=$_REQUEST["source"]?>">,
<?php if($branches[1]) { // More than one subbranch - show a select menu ?>
            <select name="subbranch">
<?php	for($i=0; $branches[$i]; $i++) { ?>
              <option value="<?=$branches[$i]?>"><?=$branches[$i]?></option>
<?php	} ?>
            </select>
<?php } else { // Only one subbranch
	if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
?>
            <?=pql_get_define("PQL_CONF_SUBTREE_USERS").",".$_REQUEST["domain"]?>
            <input type="hidden" name="subbranch" value="<?=pql_get_define("PQL_CONF_SUBTREE_USERS").",".$_REQUEST["domain"]?>">
<?php	} else { ?>
            <?=$_REQUEST["domain"]?>
            <input type="hidden" name="subbranch" value="<?=$_REQUEST["domain"]?>">
<?php	}
      }
?>
          </td>
        </tr>

        <!-- target object -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?=$LANG->_('Target DN')?></td>
          <td>
            <?php echo pql_format_error_span($error_text["destination"]); ?>
<?php // This is the ugly part (see include/attrib.administrator.inc).
	  $domains = pql_get_domains($_pql);
	  if(isset($domains)) {
	  	asort($domains);
		foreach($domains as $key => $domain) {
			// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
			$d = split(',', $domain); $d = split('=', $d[0]); $d = $d[1];
			$d = pql_maybe_decode($d);

			if($_SESSION["opera"]) {
?>
            <div id="el2Parent" class="parent" onclick="showhide(el<?=$j?>Spn, el<?=$j?>Img)">
              <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
              <font color="black" class="heada"><?=$d?></font>
            </div>

            <span id="el<?=$j?>Spn" style="display:'none'">
<?php		} else { ?>
            <div id="el<?=$j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
              </a>
        
              <a class="item">
                <font color="black" class="heada"><?=$d?></font>
              </a>
            </div>

            <div id="el<?=$j?>Child" class="child">
<?php		}
        
			// Create a user search filter (only look for mail users - !?!?).
			$filter  = "(&(".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*)(";
			$filter .= pql_get_define("PQL_ATTR_MAIL")."=*))";

			// Retreive all users in this branch/domain.
			$users   = pql_get_dn($_pql->ldap_linkid, $domain, $filter);
			if(is_array($users)) {
				// Zero out the variables, othervise we won't get users in
				// specified domain, but also in the PREVIOUS domain shown!
				unset($cns);
				foreach($users as $username) {
					$cn = pql_get_attribute($_pql->ldap_linkid, $username, pql_get_define("PQL_ATTR_CN"));
					$cns[$username] = $cn[0];
				}
				asort($cns);
				
				foreach($cns as $userdn => $username) {
?>
              <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="destination" value="<?=$userdn?>"><?=$username?></nobr><br>
<?php           	$i++;
               	}
			}

			$j++;

			if($_SESSION["opera"]) {
?>
            </span>
<?php		} else { ?>
            </div>
<?php		}
		} // end foreach domains
      } // end if domains
?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="page_curr"    value="one">
    <input type="hidden" name="page_next"    value="save">
    <input type="hidden" name="template"     value="<?=$_REQUEST["template"]?>">
    <input type="hidden" name="domain"       value="<?=$url["domain"]?>">
    <input type="hidden" name="rootdn"       value="<?=$url["rootdn"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

<?php /* $Id: user_add-alias.inc,v 1.11 2007-07-13 11:12:12 turbo Exp $ */
      // Retreive domain branches for this domain (for the subbranch value below)
      $branches = pql_unit_get($_REQUEST["domain"]);
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post" accept-charset="UTF-8">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('User data')?></th>
        <!-- source object -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Source DN')?></td>
          <td>
            <?php echo pql_format_error_span($error_text["source"]); ?>
            <?php echo pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])?>=
            <input type="text" name="source" value="<?php echo $_REQUEST["source"]?>">,
<?php if($branches[1]) { // More than one subbranch - show a select menu ?>
            <select name="subbranch">
<?php	for($i=0; $i < count($branches); $i++) { ?>
              <option value="<?php echo $branches[$i]?>"><?php echo $branches[$i]?></option>
<?php	} ?>
            </select>
<?php } else { // Only one subbranch
	if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
?>
            <?php echo pql_get_define("PQL_CONF_SUBTREE_USERS").",".$_REQUEST["domain"]?>
            <input type="hidden" name="subbranch" value="<?php echo pql_get_define("PQL_CONF_SUBTREE_USERS").",".$_REQUEST["domain"]?>">
<?php	} else { ?>
            <?php echo $_REQUEST["domain"]?>
            <input type="hidden" name="subbranch" value="<?php echo $_REQUEST["domain"]?>">
<?php	}
      }
?>
          </td>
        </tr>

        <!-- target object -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?php echo $LANG->_('Target DN')?></td>
          <td>
            <?php echo pql_format_error_span($error_text["destination"]); ?>
<?php // This is the ugly part (see include/attrib.administrator.inc).
	  $domains = pql_get_domains();
	  if(isset($domains)) {
		foreach($domains as $key => $domain) {
			// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
			$d = split(',', $domain); $d = split('=', $d[0]); $d = $d[1];
			$d = pql_maybe_decode($d);

			if($_SESSION["opera"]) {
?>
            <div id="el2Parent" class="parent" onclick="showhide(el<?php echo $j?>Spn, el<?php echo $j?>Img)">
              <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?php echo $j?>Img">
              <font color="black" class="heada"><?php echo $d?></font>
            </div>

            <span id="el<?php echo $j?>Spn" style="display:'none'">
<?php		} else { ?>
            <div id="el<?php echo $j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?php echo $j?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?php echo $j?>Img">
              </a>
        
              <a class="item">
                <font color="black" class="heada"><?php echo $d?></font>
              </a>
            </div>

            <div id="el<?php echo $j?>Child" class="child">
<?php		}
        
			// Create a user search filter
			if(pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["rootdn"]))
			  $filter = pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["rootdn"]);
			else
			  $filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";

			// Retreive all users in this branch/domain.
			$users   = $_pql->get_dn($domain, $filter);
			if(is_array($users)) {
				// Zero out the variables, othervise we won't get users in
				// specified domain, but also in the PREVIOUS domain shown!
				unset($cns);
				foreach($users as $username) {
					$cn = $_pql->get_attribute($username, pql_get_define("PQL_ATTR_CN"));
					if(is_array($cn))
					  $cns[$username] = $cn[0];
					elseif($cn)
					  $cns[$username] = $cn;
					else
					  $cns[$username] = 'unknown ('.$username.')';
				}
				asort($cns);
				
				foreach($cns as $userdn => $username) {
?>
              <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="destination" value="<?php echo $userdn?>"><?php echo $username?></nobr><br>
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
    <input type="hidden" name="template"     value="<?php echo $_REQUEST["template"]?>">
    <input type="hidden" name="domain"       value="<?php echo $url["domain"]?>">
    <input type="hidden" name="rootdn"       value="<?php echo $url["rootdn"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
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

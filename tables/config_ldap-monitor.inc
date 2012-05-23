    <table cellspacing="0" cellpadding="3" border="0">
<?php
// I won't be needing this and it's in the way...
unset($ldap['objectclasses']['ALIAS']);

foreach($ldap as $x => $array) {
?>
      <th colspan="3" align="left"><?php echo $x?>
<?php
	if(!eregi($x, 'objectclasses')) {
?>

        <tr>
          <td><?php echo $LANG->_('Name')?></td>
          <td><?php echo $LANG->_('Oid')?></td>
          <td><?php echo $LANG->_('Single valued')?></td>
          <td><?php echo $LANG->_('Description')?></td>
        </tr>

<?php    foreach($array as $value) { ?>
        <tr class="<?php pql_format_table(); ?>">
<?php       echo "          <td>".@$value['NAME']."</td>\n";
			echo "          <td>".@$value['OID']."</td>\n";
			echo "          <td>".@$value['SINGLE-VALUE']."</td>\n";
			echo "          <td>".@$value['DESC']."</td>\n";
?>
        </tr>
<?php    } ?>
      </th>

<?php
	} else {
		// Objectclasses
		$class = pql_format_table(0);
?>

        <p><font size="1"><?php echo $LANG->_('Text in bold is a MUST, and non-bold is MAY')?>.</font><br>

<?php	foreach($array as $x => $value) { ?>
        <!-- --------------------------------------------------- -->
        <!-- Objectclass #<?php echo $oc_counter?>: '<?=$value['NAME']?>:<?=$value['OID']?>' -->
        <tr class="<?php echo $class?>">
          <!-- ATTRIBUTE NAMES -->
          <td>
<?php		if($_SESSION["opera"]) { ?>
            <div id="el<?php echo $j?>Parent" class="parent" onclick="showhide(el<?=$j?>aSpn, el<?=$j?>aImg); showhide(el<?=$j?>bSpn, el<?=$j?>bImg);">
              <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?php echo $j?>aImg">
              <a class="item"><font color="black" class="heada"><?php echo $value['NAME']?></font></a>
            </div>
<?php		} else { ?>
            <div id="el<?php echo $j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?php echo $j?>', true); expandBase('el<?=$j+1?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?php echo $j?>Img">
              </a>
        
              <a class="item"><font color="black" class="heada"><?php echo $value['NAME']?></font></a>
            </div>
<?php		}

			if($_SESSION["opera"]) {
?>
            <span id="el<?php echo $j?>aSpn" style="display:''">
<?php		} else { ?>
            <div id="el<?php echo $j?>Child" class="child">
<?php		} 

			if(isset($value['MUST'])) {
			  for($i=0; $i < $value['MUST']['count']; $i++) {
				// MUST attributes
?>
              &nbsp;&nbsp;&nbsp;&nbsp;<b><?php echo $value['MUST'][$i]?></b><br>
<?php		  }
			}

			if(isset($value['MAY'])) {
			  for($i=0; $i < $value['MAY']['count']; $i++) {
				// MAY attributes
?>
              &nbsp;&nbsp;&nbsp;&nbsp;<?php echo $value['MAY'][$i]?><br>
<?php		  }
			}

			if($_SESSION["opera"]) {
?>
            </span>
<?php		} else { ?>
            </div>
<?php		} ?>
          </td>

<?php $j++; ?>
          <!-- ATTRIBUTE OIDs -->
          <td>
<?php		if($_SESSION["opera"]) { ?>
            <div id="el<?php echo $j?>Parent" class="parent" onclick="showhide(el<?=$j?>aSpn, el<?=$j?>aImg); showhide(el<?=$j?>bSpn, el<?=$j?>bImg);">
              <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?php echo $j?>bImg">
              <a class="item"><font color="black" class="heada"><?php echo $value['OID']?></font></a>
            </div>

            <span id="el<?php echo $j?>bSpn" style="display:''">
<?php		} else { ?>
            <div id="el<?php echo $j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?php echo $j-1?>', true); expandBase('el<?=$j?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?php echo $j?>Img">
              </a>
              <a class="item"><font color="black" class="heada"><?php echo $value['OID']?></font></a>
            </div>

            <div id="el<?php echo $j?>Child" class="child">
<?php		}

			if(isset($value['MUST'])) {
			  for($i=0; $i < $value['MUST']['count']; $i++) {
				// MUST attributes
?>
              &nbsp;&nbsp;&nbsp;&nbsp;<b><?php
			  	if(@$attributetypes[lc($value['MUST'][$i])]['OID']) {
				  echo $attributetypes[lc($value['MUST'][$i])]['OID'];
				} else {
				  // It's an alias
				  $alias_attribs = split(':', $value['MUST'][$i]);

				  foreach($attributetypes as $attrib) {
					if(isset($attrib['ALIAS'][0])) {
					  foreach($alias_attribs as $alias) {
						if(lc($alias) == lc($attrib['ALIAS'][0])) {
						  echo $attrib['OID'];
						}
					  }
					}
				  }
				}
			  } ?></b><br>
<?php       }

			if(isset($value['MAY'])) {
			  for($i=0; $i < $value['MAY']['count']; $i++) {
				// MAY attributes
?>
              &nbsp;&nbsp;&nbsp;&nbsp;<?php
				if(@$attributetypes[lc($value['MAY'][$i])]['OID']) {
				  echo $attributetypes[lc($value['MAY'][$i])]['OID'];
				} else {
				  // It's an alias
				  $alias_attribs = split(':', $value['MAY'][$i]);

				  foreach($attributetypes as $attrib) {
					if(isset($attrib['ALIAS'][0])) {
					  foreach($alias_attribs as $alias) {
						if(lc($alias) == lc($attrib['ALIAS'][0])) {
						  echo $attrib['OID'];
						}
					  }
					}
				  }
				} ?><br>
<?php         }
			}

			if($_SESSION["opera"]) {
?>
            </span>
<?php		} else { ?>
            </div>
<?php		} ?>
          </td>

          <!-- EMPTY TABLE -->
          <td></td>

          <!-- OBJECTCLASS DESCRIPTION -->
          <td><?php echo @$value['DESC']?></td>
        </tr>

<?php		$class = pql_format_table(0);
			$j++; $oc_counter++;
        }
	}
}
?>
      </th>
    </table>

    <!-- Arrange collapsible/expandable db list at startup -->
    <script type="text/javascript" language="javascript1.2">
    <!--
    if (isNS4) {
      firstEl  = 'el1Parent';
      firstInd = nsGetIndex(firstEl);
      nsShowAll();
      nsArrangeList();
    }
    //-->
    </script>

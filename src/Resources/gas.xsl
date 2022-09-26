<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:svg="http://www.w3.org/2000/svg">
  <xsl:template match="/">
    <html>
      <head>

        <style type="text/css">
          body {text-align: left; font-family: Verdana, Geneva, Arial, Helvetica, sans-serif; font-size: 12px;}
          h2 {font-family: "Arial"; font-size: 25px;}
          h3 {font-family: "Century Gothic"; font-size: 12px; text-align: left;}
          table {font-family: Verdana, Geneva, Arial, Helvetica, sans-serif; font-size: 11px;}
          .header_three {font-family: "Trebuchet MS"; font-size: 12px; text-align: center; vertical-align: middle; padding-left: 5px; padding-right: 5px; border: 1px solid #096361; background-color: #8A9374; font-style: normal; color: #FFFFFF; font-weight: bold;}
          .header_four {font-family: "Trebuchet MS"; font-size: 11px; text-align: center; vertical-align: middle; padding-left: 5px; padding-right: 5px; border: 1px solid #CC0000; background-color: #FFDFDF;}
          .header_one {font-family: "Trebuchet MS"; font-size: 12px; text-align: center; vertical-align: middle; padding-left: 5px; padding-right: 5px; border: 1px solid #065351; background-color: #B7E1DF; font-weight: bold;}
          .header_two {font-family: "Trebuchet MS"; font-size: 12px; text-align: center; vertical-align: middle; padding-left: 5px; padding-right: 5px; border: 1px solid Gray; background-color: #F1F1F1; font-weight: bold;}
          .line {text-align: center;}
          .line_two {background-color: #F5F5F5; text-align: center;}
            .style1
            {
                height: 17px;
            }
            .style3
            {
                font-family: "Trebuchet MS";
                font-size: 12px;
                text-align: center;
                vertical-align: middle;
                padding-left: 5px;
                padding-right: 5px;
                border: 1px solid #065351;
                background-color: #B7E1DF;
                font-weight: bold;
                height: 20px;
            }
        </style>

      </head>
      <body>
        <table width="350" border="0" align="center">
          <tr>
            <td width="150" align="center">
              <img src="{gasman/@iconPath}" width="128"/>
            </td>
            <td align="left" valign="middle">
              <h2>
                Gas Man<xsl:text disable-output-escaping="yes">&amp;reg;</xsl:text> Simulation
              </h2>
            </td>
          </tr>
        </table>

        <table width="50%" border="0" class="header_two" align="center">
          <tr>
            <td width="25%" align="left">Date/Time:</td>
            <td width="75%" align="left">
              <xsl:value-of  select="gasman/@datetime"/>
            </td>
          </tr>
          <tr>
            <td width="25%" align="left">Description:</td>
            <td width="75%" align="left">
              <xsl:value-of  select="gasman/@description"/>
            </td>
          </tr>
        </table>
        <table width="95%"  border="0" align="center" cellpadding="0" cellspacing="0">
          <tr>
            <td colspan="2">

              <hr/>
            </td>
          </tr>
          <tr>
            <td width="60%">
              <table width="100%" border="0" align="left">
                <xsl:variable name="weight">
                  <xsl:value-of select="gasman/params/patient/weight/@value" />
                </xsl:variable>
                <tr>
                  <td colspan="3" class="style3" align="center">Patient parameter </td>
                  <td align="center" colspan="2" class="style3">Flows</td>
                </tr>
                <tr>
                  <td class="style1">Weight</td>
                  <td class="style1" align="right">
                    <xsl:value-of select="$weight"/>
                  </td>
                  <td class="style1">
                    <xsl:value-of select="gasman/params/patient/weight/@unit"/>
                  </td>
                  <td class="style1"> </td>
                  <td class="style1"> </td>
                </tr>
                <xsl:variable name="fmt">
                  <xsl:choose>
                    <xsl:when test="$weight &gt;= 10">
                      <xsl:value-of select="'#0.0'"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="'#0.0000'"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:variable>
                <tr>
                  <td class="style1"  >Alveoli</td>
                  <td class="style1" align="right">
                    <xsl:value-of select="format-number(gasman/params/patient/volumes/@alv, $fmt)"/>
                  </td>
                  <td>L</td>
                  <td class="style1"> </td>
                  <td class="style1"> </td>
                </tr>

                <tr>
                  <td class="style1"  >Vessel Rich</td>
                  <td class="style1" align="right">
                    <xsl:value-of select="format-number(gasman/params/patient/volumes/@vrg, $fmt)"/>
                  </td>
                  <td>L</td>
                  <td class="style1">Vessel Rich</td>
                  <td class="style1">
                    <xsl:variable name="vrgpct" select="gasman/params/patient/flows/@vrg"/>
                    <xsl:value-of select='format-number($vrgpct, "##%")' />
                  </td>
                </tr>

                <tr>
                  <td class="style1"  >Muscle</td>
                  <td class="style1" align="right">
                    <xsl:value-of select="format-number(gasman/params/patient/volumes/@mus, $fmt)"/>
                  </td>
                  <td>L</td>
                  <td class="style1">Muscle</td>
                  <td class="style1">
                    <xsl:variable name="muspct" select="gasman/params/patient/flows/@mus"/>
                    <xsl:value-of select='format-number($muspct, "##%")' />
                  </td>
                </tr>
                <tr>
                  <td class="style1">Fat</td>
                  <td class="style1" align="right">
                    <xsl:value-of select="format-number(gasman/params/patient/volumes/@fat, $fmt)"/>
                  </td>
                  <td>L</td>
                  <td class="style1">Fat</td>
                  <td class="style1">
                    <xsl:variable name="fatpct" select="gasman/params/patient/flows/@fat"/>
                    <xsl:value-of select='format-number($fatpct, "##%")' />
                  </td>
                </tr>
                <tr>
                  <td class="style1">Venous</td>
                  <td class="style1" align="right">
                    <xsl:value-of select="format-number(gasman/params/patient/volumes/@ven, $fmt)"/>
                  </td>
                  <td>L</td>
                  <td> </td>
                  <td> </td>
                </tr>
              </table>
            </td>
            <td width="40%" align="left" valign="top">
              <table width="100%"  border="0" align="right">
                <tr>
                  <td height="20" colspan="6" class="header_one" align="center">Anesthetic(s)</td>
                </tr>
                <tr>
                  <td width="25%" align="center" class="header_two">Name</td>
                  <td colspan="4" align="center" class="header_two">Solubility</td>
                  <td align="center" class="header_two">Volatility</td>
                </tr>
                <tr>
                  <td > </td>
                  <td  align="center" class="header_two">Blood</td>
                  <td  align="center" class="header_two">VRG</td>
                  <td  align="center" class="header_two">Mus</td>
                  <td  align="center" class="header_two">Fat</td>
                  <td  align="center"> </td>
                </tr>
                <xsl:for-each select="gasman/params/agents/agent">
                  <tr>
                    <td >
                      <xsl:value-of select="@name"/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select='format-number(@lambdaBlood, "#0.##")'/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select='format-number(@lambdaVrg, "#0.##")'/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select='format-number(@lambdaMus, "#0.##")'/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select='format-number(@lambdaFat, "##0.##")'/>
                    </td>
                    <td  align="center">
                      <xsl:value-of select="@volatility"/>
                    </td>
                  </tr>
                </xsl:for-each>
              </table>
            </td>
          </tr>
        </table>
        <hr width="95%"/>
        <xsl:variable name="shouldPrintPrintout">
          <xsl:value-of select="gasman/@printPrintout" />
        </xsl:variable>
        <xsl:if test="$shouldPrintPrintout != 'false'">
          <table width="95%"  border="0" align="center">
            <tr>
              <td class="header_three" align="center">Time</td>
              <td class="header_three" align="center">Agent</td>
              <td class="header_three" align="center">Vaporizer (%atm)</td>
              <td class="header_three" align="center">Injected</td>
              <td class="header_three" align="center">Circuit</td>
              <td class="header_three" align="center">Fresh Gas (L/m)</td>
              <td class="header_three" align="center"> Alveolar Ventil'n (L/m)</td>
              <td class="header_three" align="center"> Cardiac Output (L/m)</td>
            </tr>
            <xsl:for-each select="gasman/settings/setting">
              <xsl:if test="count(agentsettings/agentsetting) = 1">
                <tr class="line_two">
                  <td align="center">
                    <xsl:value-of select="@time"/>
                  </td>
                  <td align="center" class="line">
                    <xsl:value-of select="agentsettings/agentsetting[1]/@agentname"/>
                  </td>
                  <td align="center" class="line">
                    <xsl:value-of select='format-number(agentsettings/agentsetting[1]/@del, "##0.##")'/>
                  </td>
                  <td align="center" class="line">
                    <xsl:value-of select='format-number(agentsettings/agentsetting[1]/@inject, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select="@circuit"/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@fgf, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@va, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@co, "##0.##")'/>
                  </td>
                </tr>
              </xsl:if>
              <xsl:if test="count(agentsettings/agentsetting) != 1">
                <tr class="line_two">
                  <td align="center">
                    <xsl:value-of select="@time"/>
                  </td>
                  <td align="center"/>
                  <td align="center"/>
                  <td align="center"/>
                  <td align="center">
                    <xsl:value-of select="@circuit"/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@fgf, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@va, "##0.##")'/>
                  </td>
                  <td align="center">
                    <xsl:value-of select='format-number(@co, "##0.##")'/>
                  </td>
                </tr>
                <xsl:for-each select="agentsettings/agentsetting">
                  <tr>
                    <td align="center" class="line"/>
                    <td align="center" class="line">
                      <xsl:value-of select="@agentname"/>
                    </td>
                    <td align="center" class="line">
                      <xsl:value-of select='format-number(@del, "##0.##")'/>
                    </td>
                    <td align="center" class="line">
                      <xsl:value-of select='format-number(@inject, "##0.##")'/>
                    </td>
                  </tr>
                </xsl:for-each>
              </xsl:if>
            </xsl:for-each>
          </table>
        </xsl:if>
        <xsl:variable name="shouldPrintGraph">
          <xsl:value-of select="gasman/@printGraph" />
        </xsl:variable>
        <xsl:if test="$shouldPrintGraph != 'false'">
          <div align="center">
            <img>
              <xsl:attribute name="src">
                <xsl:value-of select="gasman/@externalImage" />
              </xsl:attribute>
            </img>
          </div>
        </xsl:if>
      </body>
    </html>

  </xsl:template>
</xsl:stylesheet>

# Safeguard secret

This plugin enables you to use Milagro-D-TA to encrypt a string using milagro-crypto-c

**You can see the API spec for this plugin in safeguardsecret-api.yaml**

## Encrypt a String

**Put In Your D-TA server's ID Doc IPFS address**

```
curl -X POST "http:localhost:5556/order" -H "accept: */*" -H "Content-Type: application/json" -d "{\"BeneficiaryIDDocumentCID\":\"QmaJAj18pABdfU777mhv3rBmAqZRbE6vr1JgrZ2BcXwWj3\",\"Extension\":{\"plainText\":\"Let's encrypt s0m3 d@t@\"}}"
```



## Decrypt a String

**Swap In the correct Values from above**
```
curl -X POST "http:localhost:5556/order/secret" -H "accept: */*" -H "Content-Type: application/json" -d "{\"OrderPart2CID\":\"QmXDdLPoeczWxzNFyxgSDkqxw71pnvBqB1xdW7XnyAPMJo\",\"BeneficiaryIDDocumentCID\":\"QmaJAj18pABdfU777mhv3rBmAqZRbE6vr1JgrZ2BcXwWj3\",\"Extension\":{\"cypherText\":\"3752ca1032ddba51b9525833550d509def1a9692d910b048172e7406ad40cbf1\",\"t\":\"b1e72740d16a6587496ec563\",\"v\":\"04dac12c6648f2c8e2f8c8522c46c70fdc0fe37f43ad855a11cb66132f1fab75ab1b8d9c1594a20c0dc947ef604e7339cbbe05a2d59965e0183bbf8d59a4f4821c\"}}"
```

## To Test the Plugin Code

```
cd ~/go/src/github.com/apache/incubator-milagro-dta/pkg/safeguardsecret

go test

```




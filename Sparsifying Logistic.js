var v=require('./vector').vector;
var m=require('./matrix').matrix;
var sparse= {
    softness:10, sparseness:0.01,
    softmax:function(data){
        return this.sparseness*Math.exp(this.softness*data)
    },
    single:function(input,s_ini){
        var z=[],s=[];
        for(var i=0;i<input.length;i++){
            if(!s[i-1]) {
                if(!s_ini) s[i-1]= v.sum(input)/input.length;
                else       s[i-1]=s_ini;
            }
            s.push(this.softmax(input[i])+(1-this.sparseness)*s[i-1]);
            var res=(this.softmax(input[i])/s[i]).toFixed(3);
            if(res<0.1) res=0;
            z.push(res);
        }
        return z;
    },
    sample:function(sample){
        var t= m.T(sample),sum= 0,num= 0,res=[];
        m.walk(t,function(i,j,v){
            sum+=v;
            num++;
        });
        for(var i=0;i<t.length;i++){ res.push(this.single(t[i],sum/num)) }
        return m.T(res);
    }
}
exports.sparse=sparse;
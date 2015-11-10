var data=require('./data').data;
var vector=data.vector;
var weight=data.weight;
var network=require('./network').network;
var sigmoid=require('./sigmoid').sigmoid;

var BP={
    create_obj:function(input,num_hidden,target,lim,range){
        var obj={
            i:input,t:target||input,
            lim:lim||0.01,
            num:{ i:input.length, j:num_hidden,k:(target||input).length},
            weight:BP.ini_weight(input.length,num_hidden,(target||input).length,range)
        };
        return obj;
    },
    ini_weight:function(num_input,num_hidden,num_output,range){
        if(range==undefined) range=[-1,1];
        return {
            ij:data.weight.get([num_hidden,num_input+1],range),
            jk:data.weight.get([num_output,num_hidden+1],range),
            prior:data.weight.get([num_hidden,num_input+1],0)
        }
    },
    active:function(obj){
        if(!obj.sigmoid) obj.sigmoid=sigmoid.BP;
        if(obj.i[obj.i.length-1]!==-1) obj.i.push(-1);
        var hidden=vector.func(network.active(obj.i,obj.weight.ij),obj.sigmoid);
        if(hidden[hidden.length-1]!==-1) hidden.push(-1);
        var output=vector.func(network.active(hidden,obj.weight.jk),obj.sigmoid);
        obj.i=obj.i.slice(0,obj.i.length-1);
        obj.j=hidden.slice(0,hidden.length-1);
        obj.k=output;
        if(obj.t.length==obj.num.k+1) obj.t=obj.t.slice(0,obj.t.length-1);
        obj.error=vector.interact(obj.t,obj.k,'-');
        return obj;
    },
    correct:{
        jk:function(obj,l){
            if(!l) l=1;
            var Ek=vector.interact(obj.error,vector.func(obj.k,sigmoid.XOR),'*');
            obj.weight.jk=weight.walk(obj.weight.jk,function(data){
                obj.weight.jk[data.j][data.i]+=l*Ek[data.j]*(obj.j[data.i]||(-1));
            });
            return obj;
        },
        ij:function(obj,l){
            if(!l) l=1;
            var Ek=vector.interact(obj.error,vector.func(obj.k,sigmoid.XOR),'*');
            var e=network.active(Ek,weight.T(obj.weight.jk));
            var Ej=vector.interact(vector.func(obj.j,sigmoid.XOR), e.slice(0, e.length-1),'*');
            obj.weight.ij=weight.walk(obj.weight.ij,function(data){
                obj.weight.ij[data.j][data.i]+=l*Ej[data.j]*(obj.i[data.i]||(-1))+0.9*obj.weight.prior[data.j][data.i];
                obj.weight.prior[data.j][data.i]=l*Ej[data.j]*(obj.i[data.i]||(-1));
            });
            return obj;
        }
    },
    error:function(obj){
        var s=vector.sum;
        var i=vector.interact;
        return s(i(i(obj.error, obj.error,'*'),0.5,'*'));
    },
    train:{
        single:function(obj){
            var times=0;
            for(;;){
                obj=BP.active(obj);
                if(BP.error(obj)<=obj.lim){
                    obj.times=times;
                    obj.weight.prior=data.weight.get([obj.num.j,obj.num.i+1],0);
                    return obj;
                }
                else{
                    times++;
                    o=BP.correct.jk(obj,1);
                    o=BP.correct.ij(obj,1);
                }
            }
        },
        sample:function(sample,obj){
            for(;;){
                var sum_times=0;
                for(var i=0;i<sample.length;i++){
                    obj.i=sample[i].input||sample[i];
                    obj.t=sample[i].target||obj.i;
                    obj=BP.train.single(obj);
                    sum_times+=obj.times;
                }
                if(sum_times==0) return obj;
            }
        }
    }
}
exports.BP=BP;

var sample=weight.get([2,10],[0,1]);
var obj=BP.create_obj(sample[0],16);
obj=BP.train.sample(sample,obj);
console.log(obj);
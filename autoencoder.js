var v=require('./vector').vector;
var m=require('./matrix').matrix;
var a=require('./algorithm').algorithm;
var BP=require('./BP').BP;
var autoencoder={
    layer:[],weight:[],
    create:function(vec){
        for(var i= vec.length-2;i>=0;i--) vec.push(vec[i]);
        this.layer=vec;
        this.weight= v.create(vec.length-1,0);
    },
    BP:function(){
        var sample=[[0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9],[0.2,0.4,0.6,0.8,0.1,0.3,0.5,0.7,0.9]];
        autoencoder.create([9,6,4,2]);
        for(var i=0;i<(this.layer.length-1)/2;i++){
            BP.create(this.layer[i],this.layer[i+1],this.layer[i]);
            BP.train.sample(sample);
            this.weight[i]= BP.weight.ij;
            this.weight[this.weight.length-i-1]= BP.weight.jk;
            for(var j=0;j<sample.length;j++) {
                if(v.last(sample[j])==-1) sample[j]= v.delete_last(sample[j]);
                sample[j]= BP.import(sample[j]).j;
            }
            if(i==(this.layer.length-1)/2-1) console.log(sample);
        }
    },
    energy_based:function(){}
}
exports.autoencoder=autoencoder;
autoencoder.BP();
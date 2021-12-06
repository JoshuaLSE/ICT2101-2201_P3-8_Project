using System;
using Microsoft.EntityFrameworkCore;

namespace CodeACar.Models
{
    public class Challenge
    {
        public int ChallengeId { get; set; }
        public string Name { get; set; }
        public string ChallengeImage { get; set; }

        public string Solution { get; set; }
    }
}
